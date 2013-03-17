#include "project.hpp"

#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>

#include <QDomDocument>

namespace Developer {

Project::Project(const QString &projectPath, QObject *parent)
    : GPFile(projectPath, parent)
    , _gpVersion(DEFAULT_GP_VERSION)
    , _gpDeveloperVersion(GP_DEVELOPER_VERSION)
    , _name("")
    , _error("")
{
}

Project::~Project()
{

}

QString Project::name() const
{
    return _name;
}

void Project::setName(const QString &name)
{
    _name = name;
}

GPVersions Project::gpVersion() const
{
    return _gpVersion;
}

void Project::setGPVersion(GPVersions version)
{
    _gpVersion = version;
}

QString Project::error() const
{
    return _error;
}

QDir Project::rulesDir() const
{
    QDir d = dir();
    d.cd("rules");

    // This directory should always exist, if it doesn't then make it again
    if(!d.exists())
        d.mkpath(d.absolutePath());

    return d;
}

QDir Project::programsDir() const
{
    QDir d = dir();
    d.cd("programs");

    // This directory should always exist, if it doesn't then make it again
    if(!d.exists())
        d.mkpath(d.absolutePath());

    return d;
}

QDir Project::graphsDir() const
{
    QDir d = dir();
    d.cd("graphs");

    // This directory should always exist, if it doesn't then make it again
    if(!d.exists())
        d.mkpath(d.absolutePath());

    return d;
}

bool Project::open()
{
    return GPFile::open();
}

bool Project::open(const QString &projectPath)
{
    _path = projectPath;
    if(!open())
    {
        _error = tr("The project specified (%1) could not be opened"
                    ).arg(projectPath);
        return false;
    }

    // We need the file to exist to proceed, fail if the status is not Normal
    if(_status != GPFile::Normal)
    {
        _error = tr("The project specified (%1) did not exist"
                    ).arg(projectPath);
        return false;
    }

    // Parse the file, details of the format are in the documentation for the
    // Project class
    QString proj = _fp->readAll();
    if(proj.isEmpty())
    {
        _error = tr("The project specified (%1) was empty"
                    ).arg(projectPath);
        return false;
    }

    QDomDocument document("project");
    if(!document.setContent(proj))
    {
        _error = tr("The project file (%1) could not be parsed as XML"
                    ).arg(projectPath);
        return false;
    }

    // First get the top-level <project> element, fail if it cannot be found
    QDomNodeList nodes = document.elementsByTagName("project");
    if(nodes.isEmpty())
    {
        _error = tr("The project file (%1) provided could not be parsed as a GP"
                    "Developer project"
                    ).arg(projectPath);
        return false;
    }

    // Take the node returned, make it into an element, and then grab all of the
    // attributes associated with it
    QDomNode node = nodes.at(0);
    QDomElement elem = node.toElement();
    QString name = elem.attribute("name");
    QString directory = elem.attribute("directory");
    QString gpVersion = elem.attribute(
                "gpversion",
                GPVersionToString(DEFAULT_GP_VERSION)
                );
    QString gpDeveloperVersion = elem.attribute(
                "developerversion",
                QVariant(GP_DEVELOPER_VERSION).toString()
                );

    // A name and a target directory must be present, in addition the directory
    // provided must exist
    if(name.isEmpty() || directory.isEmpty()
            || !QDir(directory).exists())
    {
        _error = tr("The project file (%1) provided could not be parsed as a GP"
                    "Developer project"
                    ).arg(projectPath);
        return false;
    }

    _error = "";
    return true;
}

bool Project::initProject(const QString &targetPath, const QString &projectName, GPVersions gpVersion)
{
    QDir dir(targetPath);
    if(!dir.isAbsolutePath(targetPath))
        return false;

    if(!dir.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, tr("Create Directory?"),
                                      tr("The directory specified (%1) does not"
                                         " exist, create it?").arg(targetPath),
                                      QMessageBox::Yes | QMessageBox::Cancel
                                      );
        if(reply != QMessageBox::Yes)
            return false;

        dir.mkpath(targetPath);
    }

    // Check for subdirectories called "rules", "programs" or "graphs" - create
    // them automatically if they don't exist.
    QString path = targetPath;
    if(path.at(targetPath.length()-1) != dir.separator())
        path += QString(dir.separator());
    QString rulesPath = path + "rules";
    QString programsPath = path + "programs";
    QString graphsPath = path + "graphs";

    QDir rules(rulesPath);
    if(!rules.exists())
        rules.mkpath(rulesPath);

    QDir programs(programsPath);
    if(!programs.exists())
        programs.mkpath(programsPath);

    QDir graphs(graphsPath);
    if(!graphs.exists())
        graphs.mkpath(graphsPath);

    QFile file(dir.filePath(projectName + GP_PROJECT_EXTENSION));
    _path = dir.filePath(projectName + GP_PROJECT_EXTENSION);
    file.open(QFile::WriteOnly);

    setName(projectName);

    /*
     * Write a basic template project file
     *
     * Replacements:
     *  %1 => Project name
     *  %2 => Project working directory
     *  %3 => GP version
     *  %4 => GP Developer version
     */
    QFile fp(":/templates/newproject.gpp");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString newProject = fp.readAll();
    newProject = newProject.arg(
                projectName,
                targetPath,
                GPVersionToString(gpVersion),
                QVariant(GP_DEVELOPER_VERSION).toString()
                );

    // First just open up the file, don't parse it
    if(!open())
        return false;

    _fp->write(QVariant(newProject).toByteArray());

    // Now we can parse it
    open(_path);

    return true;
}

void Project::newRule(const QString &name)
{
    // Have we been passed an absolute path?
    //! \todo handle this

    // Get the directory in which we should be creating this rule
    QDir d = rulesDir();

    // Check if a rule with this name already exists
    QString filePath = d.filePath(name + GP_RULE_EXTENSION);
    QFile file(filePath);

    if(file.exists())
    {
        QMessageBox::StandardButton response;
        response = QMessageBox::warning(
                    this,
                    tr("File Exists"),
                    tr("A file called \"%1\" already exists, do you want to"
                       "overwrite it?").arg(filePath),
                    QMessageBox::Yes | QMessageBox::Cancel
                    );
        // The user hit cancel, bail without creating a rule
        if(response != QMessageBox::Yes)
            return;
    }

    file.open(QFile::ReadWrite);
    file.write(QVariant(
                   QString("/* Rule: %1, created at: %2").arg(
                       name,
                       QDateTime::currentDateTime().toString("dd/MM/YYYY hh:mm:ss")
                       )
                   ).toByteArray());

    // Add the resulting file
    addRule(filePath);
}

void Project::newProgram(const QString &name)
{
    // Make the program

    // Add the resulting program
    //addProgram();
}

void Project::newGraph(const QString &name, GraphTypes type)
{
    // Make the graph

    // Add the resulting graph
    //addGraph();
}

void Project::addRule(const QString &path)
{
    QFile f(path);
    if(!f.exists())
        return;

    Rule *rule = new Rule(path, this);
    _rules.push_back(rule);
}

void Project::addProgram(const QString &path)
{

}

void Project::addGraph(const QString &path)
{

}

void Project::setCurrentFile(const QString &fileName, FileTypes type)
{

}

bool Project::save()
{
    return false;
}

bool Project::saveAs(const QString &filePath)
{
    return GPFile::saveAs(filePath);
}

bool Project::saveFile(QString filePath)
{
    return false;
}

bool Project::saveFileAs(QString filePath)
{
    return false;
}

bool Project::saveAll()
{
    return false;
}

void Project::fileModified(QString filePath)
{

}

}
