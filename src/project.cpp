#include "project.hpp"

#include <QDir>
#include <QMessageBox>
#include <QFile>

#include <QDomDocument>

Project::Project(const QString &projectPath, QObject *parent)
    : GPFile(projectPath, parent)
    , _gpVersion(DEFAULT_GP_VERSION)
    , _gpDeveloperVersion(GP_DEVELOPER_VERSION)
    , _currentFile(0)
    , _watcher(0)
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

bool Project::openProject(const QString &projectPath)
{
    QFile project(projectPath);
    if(!project.exists())
    {
        _error = tr("The project specified (%1) did not exist").arg(projectPath);
        return false;
    }

    _projectFile = new QFile(projectPath);
    _projectFile->open(QFile::ReadOnly);

    if(_watcher != 0)
        delete _watcher;

    // Construct a QStringList of files to watch as we read in files
    QStringList watchDirs;

    // Parse the file, details of the format are in the documentation for the
    // Project class
    QString proj = _projectFile->readAll();
    if(proj.isEmpty())
    {
        _error = tr("The project specified (%1) was empty").arg(projectPath);
        return false;
    }

    QDomDocument document("project");
    if(!document.setContent(proj))
    {
        _error = tr("The project file (%1) could not be parsed as XML").arg(projectPath);
        return false;
    }

    // First get the top-level <project> element, fail if it cannot be found
    QDomNodeList nodes = document.elementsByTagName("project");
    if(nodes.isEmpty())
    {
        _error = tr("The project file (%1) provided could not be parsed as a GP"
                    "Developer project").arg(projectPath);
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
                    "Developer project").arg(projectPath);
        return false;
    }

    // Set up a watcher to detect changes to project files
    _watcher = new QFileSystemWatcher(watchDirs, this);
    connect(_watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));

    // Success, clean up and exit
    _projectFile->close();
    _error = "";
    return true;
}

bool Project::initProject(const QString &targetPath, const QString &projectName)
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

    QFile file(dir.filePath(projectName + ".gpp"));
    _path = dir.filePath(projectName + ".gpp");
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
                projectName
                );

    openProject(_path);

    return true;
}

void Project::newRule(const QString &name)
{
    // Make the rule

    // Add the resulting file
    //addRule();
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

bool Project::saveAs(const QString &path)
{
    return false;
}

bool Project::saveFile(QString file)
{
    return false;
}

bool Project::saveFileAs(QString file)
{
    return false;
}

bool Project::saveAll()
{
    return false;
}

bool Project::import()
{
    return false;
}

bool Project::import(QString file)
{
    return false;
}

void Project::fileChanged(QString file)
{

}

const QString GPVersionToString(GPVersions version)
{
    switch(version)
    {
        case GP1:
            return QString("gp1");
        case GP2:
            return QString("gp2");
        case RootedGP2:
            return QString("rootedgp2");
        default:
            qDebug() << "Unknown version passed: " << version;
            return QString("");
    }
}

GPVersions stringToGPVersion(const QString &version)
{
    if(version == "gp1")
        return GP1;

    if(version == "gp2")
        return GP2;

    if(version == "rootedgp2")
        return RootedGP2;

    qDebug() << "Unknown version string passed: " << version;
    qDebug() << "Defaulting to Rooted GP2";
    return RootedGP2;
}
