/*!
 * \file
 */
#include "welcome.hpp"
#include "ui_welcome.h"

#include "project.hpp"

#include <QFile>
#include <QSignalMapper>

namespace Developer {

Welcome::Welcome(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Welcome)
    , _mapper(0)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/welcome.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->newProjectButton->setMainText(tr("New Project"));
    _ui->newProjectButton->setSubtext(tr("Create a new GP project"));

    _ui->openProjectButton->setMainText(tr("Open Project"));
    _ui->openProjectButton->setSubtext(tr("Open an existing GP project"));
}

Welcome::~Welcome()
{
    delete _ui;
}

void Welcome::recentProjectsUpdated(QStringList projects)
{
    if(_mapper != 0)
    {
        _mapper->disconnect();
        delete _mapper;
    }

    QLayoutItem *item;
    while((item = _ui->recentProjectsGroup->layout()->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }

    if(projects.count() < 1)
    {
        // No recent projects
        _ui->recentProjectsGroup->layout()->addWidget(
                    new QLabel(tr("No recent projects"), _ui->recentProjectsGroup)
                    );
    }
    else
    {
        _mapper = new QSignalMapper(this);

        // There are recent projects, add them to the widget
        for(QStringList::iterator iter = projects.begin();
            iter != projects.end(); ++iter)
        {
            // Attempt to make this into a project, we need to be able to get a
            // name
            Project *proj = new Project(*iter, false);
            if(proj->name().isEmpty())
                continue;

            StyledButton *button = new StyledButton(_ui->recentProjectsGroup);
            button->setMainText(proj->name());
            button->setSubtext(proj->absolutePath());
            connect(button, SIGNAL(pressed()), _mapper, SLOT(map()));
            _mapper->setMapping(button, proj->absolutePath());

            delete proj;
            _ui->recentProjectsGroup->layout()->addWidget(button);
        }

        connect(_mapper, SIGNAL(mapped(QString)),
                this, SLOT(openProject(QString)));
    }
}

void Welcome::newProject()
{
    emit newProjectClicked();
}

void Welcome::openProject(QString path)
{
    if(path.isEmpty())
        emit openProjectClicked();
    else
        emit openProjectClicked(path);
}

}
