#include "welcome.hpp"
#include "ui_welcome.h"

#include <QFile>

Welcome::Welcome(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Welcome)
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

    _ui->recentProject1->setMainText(tr("Project_1"));
    _ui->recentProject1->setSubtext("C:\\Users\\Alex\\GP2\\Project_1\\Project_1.gpp");
}

Welcome::~Welcome()
{
    delete _ui;
}

void Welcome::newProject()
{
    emit newProjectClicked();
}

void Welcome::openProject(QString path)
{
    if(path == QString())
        emit openProjectClicked();
    else
        emit openProjectClicked(path);
}
