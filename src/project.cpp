#include "project.hpp"

#include <QDir>
#include <QMessageBox>

Project::Project(const QString &projectPath, QObject *parent)
    : QObject(parent)
    , _projectPath(projectPath)
    , _currentFile(0)
{

}

Project::~Project()
{

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

    QFile file(dir.filePath(projectName + ".gpproj"));
    file.open(QFile::WriteOnly);

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
