/*!
 * \file
 */
#include "rule.hpp"
#include "ruleparser.hpp"

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
{
    if(!rulePath.isEmpty())
        open();
}

QString Rule::name() const
{
    return _name;
}

void Rule::setName(const QString &ruleName)
{
    _name = ruleName;
}

bool Rule::save()
{
    return true;
}

bool Rule::saveAs(const QString &filePath)
{
    return GPFile::saveAs(filePath);
}

bool Rule::open()
{
    if(!GPFile::open())
        return false;

    QString ruleContents = _fp->readAll();
    if(ruleContents.isEmpty())
        return false;

    std::string ruleString = ruleContents.toStdString();
    rule_t rule = parseRule(ruleString);

    setName(rule.id.c_str());

    qDebug() << rule;

    return true;
}

}
