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

bool Project::initProject()
{
    QDir dir(_projectPath);
    if(!dir.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, tr("Create Directory?"),
                                      tr("The directory specified (%1) does not"
                                         "exist, create it?").arg(_projectPath),
                                      QMessageBox::Yes | QMessageBox::Cancel
                                      );
        if(reply != QMessageBox::Yes)
            return false;
    }

    return true;
}


