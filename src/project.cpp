#include "project.hpp"

#include <QDir>
#include <QMessageBox>
#include <QFile>

Project::Project(const QString &projectPath, QObject *parent)
    : QObject(parent)
    , _gpVersion(DEFAULT_GP_VERSION)
    , _gpDeveloperVersion(GP_DEVELOPER_VERSION)
    , _projectPath(projectPath)
    , _currentFile(0)
    , _watcher(0)
{

}

Project::~Project()
{

}

bool Project::openProject(const QString &projectPath)
{
    QFile project(projectPath);
    if(!project.exists())
        return false;

    _projectFile = new QFile(projectPath);
    _projectFile->open(QFile::ReadOnly);

    if(_watcher != 0)
        delete _watcher;

    // Construct a QStringList of files to watch as we read in files
    QStringList watchDirs;

    // Parse the file, details of the format are in the documentation for the
    // Project class

    _watcher = new QFileSystemWatcher(watchDirs, this);
    connect(_watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
    _projectFile->close();

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
    _projectPath = dir.filePath(projectName + ".gpp");
    file.open(QFile::WriteOnly);

    openProject(_projectPath);

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

void Project::newGraph(const QString &name, Graph::GraphTypes type)
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

bool Project::save(QString file)
{
    return false;
}

bool Project::saveAs(QString file)
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
