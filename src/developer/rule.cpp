/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "rule.hpp"
#include "ruleparser.hpp"

#include "graph.hpp"

#include <QFileDialog>
#include <QDebug>
#include <QRegExp>

namespace Developer {

Rule::Rule(const QString &rulePath, QObject *parent)
    : GPFile(rulePath, parent)
    , _name("")
    , _documentation("")
    , _lhs(0)
    , _rhs(0)
    , _condition("")
    , _options(Rule_DefaultBehaviour)
{
    if(!rulePath.isEmpty())
        open();
}

const QString &Rule::name() const
{
    return _name;
}

const QString &Rule::documentation() const
{
    return _documentation;
}

Graph *Rule::lhs() const
{
    return _lhs;
}

Graph *Rule::rhs() const
{
    return _rhs;
}

const QString &Rule::condition() const
{
    return _condition;
}

int Rule::options() const
{
    return _options;
}

bool Rule::injectiveMatching() const
{
    return (_options & Rule_InjectiveMatching);
}

void Rule::setName(const QString &ruleName)
{
    if(ruleName.isEmpty() || ruleName == _name)
        return;

    _name = ruleName;
    _status = Modified;
    emit statusChanged(_status);
}

void Rule::setDocumentation(const QString &docString)
{
    if(docString.isEmpty() || docString == _documentation)
        return;

    _documentation = docString;
    _status = Modified;
    emit statusChanged(_status);
}

void Rule::setLhs(Graph *lhsGraph)
{
    _lhs = lhsGraph;
    connect(_lhs, SIGNAL(statusChanged(FileStatus)), this, SLOT(lhsGraphChanged()));
    _status = Modified;
    emit statusChanged(_status);
}

void Rule::setRhs(Graph *rhsGraph)
{
    _rhs = rhsGraph;
    connect(_lhs, SIGNAL(statusChanged(FileStatus)), this, SLOT(rhsGraphChanged()));
    _status = Modified;
    emit statusChanged(_status);
}

void Rule::setCondition(const QString &conditionString)
{
    if(conditionString.isEmpty() || conditionString == _condition)
        return;

    _condition = conditionString;
    _status = Modified;
    emit statusChanged(_status);
}

void Rule::setOptions(int options)
{
    // Check if this is valid input, remove all known option settings and if any
    // bits are still set then this is invalid and we should warn the user.
    int copy = options;
    copy &= ~Rule_InjectiveMatching;

    if(copy)
    {
        qDebug() << "Value passed to Rule::setOptions was not a valid "
                    "combination of settings.";
        return;
    }
    else
        _options = options;
}

void Rule::setInjectiveMatching(bool injective)
{
    if(injective)
        _options |= Rule_InjectiveMatching;
    else
        _options &= ~Rule_InjectiveMatching;
}

bool Rule::save()
{
    // Some initial sanity checks
    if(_path.isEmpty() || !_fp->isOpen())
        return false;

    _fp->close();
    ++_internalChanges;
    _fp->open(QFile::Truncate | QFile::WriteOnly);
    qDebug() << "Saving rule file: " << _fp->fileName();

    // Construct the save file, this means making the documentation into a
    // comment and then concatenating the program contents
    QString docText = _documentation;
    docText.replace("\n","\n * ");
    QString saveText = QString("/*!\n * ") + docText + "\n */\n";

    saveText += _name + "(";

    // Collect variables here

    saveText += ") = {\n";
    saveText += _lhs->toAlternative();
    saveText += "\n}\n=> {\n";
    saveText += _rhs->toAlternative();
    saveText += "}\n";
    saveText += "interface={";

    // Write interface here

    saveText += "}\nwhere ";
    saveText += _condition;
    saveText += "\n";

    ++_internalChanges;
    int status = _fp->write(QVariant(saveText).toByteArray());
    if(status <= 0)
    {
        qDebug() << "    Save failed";
        return false;
    }

    _fp->close();
    _fp->open(QFile::ReadWrite);

    qDebug() << "    Save completed. Wrote " << status << " bytes";

    _status = Normal;
    emit statusChanged(_status);
    return true;
}

bool Rule::saveAs(const QString &filePath)
{
    QString thePath = filePath;
    if(filePath.isEmpty())
    {
        QDir d = dir();
        QString dirPath;
        if(d.path().isEmpty())
            dirPath = QDir::homePath();
        else
            dirPath = d.absolutePath();

        thePath = QFileDialog::getSaveFileName(
                    0,
                    tr("Save Rule As..."),
                    dirPath,
                    tr("GP Rules (*.gpr)"));
        if(thePath.isEmpty())
            return false;
    }

    // Cache the path to the old file, if the save process fails then we should
    // restore the old one
    QString pathCache = _path;
    _path = thePath;
    open();
    if(!save())
    {
        // The save process failed
        qDebug() << "Rule could not be saved to " << filePath;
        qDebug() << "Reopening previous file.";
        _path = pathCache;
        open();
        return false;
    }

    // Update the file watcher
    bool ret = GPFile::saveAs(_path);

    // Delete the old file as the move was successful
    QFile(pathCache).remove();

    return ret;
}

bool Rule::open()
{
    if(!GPFile::open())
        return false;

    qDebug() << "Opening rule file: " << _path;

    QString ruleContents = _fp->readAll();
    if(ruleContents.isEmpty())
        return false;

    std::string ruleString = ruleContents.toStdString();
    rule_t rule = parseRule(ruleString);

    qDebug() << "    Finished parsing rule file: " << _path;

    QString docString = rule.documentation.c_str();
    // Strip opening whitespace and the first * if one exists, this allows for
    // common C/C++/Java-style multiline comments such as the top of this file
    docString.replace(QRegExp("\n\\s*\\*\\s*"), "\n");
    docString = docString.trimmed();

    setName(rule.id.c_str());
    setDocumentation(docString);
    if(rule.lhs.is_initialized())
        setLhs(new Graph(rule.lhs.get(), this));
    else
        setLhs(new Graph(QString(), false, this));
    if(rule.rhs.is_initialized())
        setRhs(new Graph(rule.rhs.get(), this));
    else
        setRhs(new Graph(QString(), false, this));
    if(rule.condition.is_initialized())
        setCondition(rule.condition.get().c_str());
    else
        setCondition(QString());

    _status = Normal;
    emit statusChanged(_status);

    return true;
}

void Rule::lhsGraphChanged()
{
    _status = Modified;
    emit statusChanged(_status);
}

void Rule::rhsGraphChanged()
{
    _status = Modified;
    emit statusChanged(_status);
}

}
