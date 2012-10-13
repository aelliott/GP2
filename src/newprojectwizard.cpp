#include "newprojectwizard.hpp"
#include "ui_newprojectwizard.h"

#include <QDir>

NewProjectWizard::NewProjectWizard(QWidget *parent)
    : QWizard(parent)
    , _ui(new Ui::NewProjectWizard)
    , _project(0)
{
    _ui->setupUi(this);

#ifndef Q_OS_WIN32
    // The Linux and OSX styles do not fit in with the rest of the application,
    // however the Aero style looks pretty good with the existing stylesheet.
    setWizardStyle(QWizard::ModernStyle);
#endif // Q_OS_WIN32

    _ui->projectLocationEdit->setText(QDir::homePath());
}

NewProjectWizard::~NewProjectWizard()
{
    delete _ui;
}

Project *NewProjectWizard::project() const
{
    return _project;
}

void NewProjectWizard::updateProjectLocation()
{
    QString location = _ui->projectLocationEdit->text();
    // A bit over-simplistic, should account for \ on Windows
    if(!location.endsWith("/"))
        location += "/";

    location += _ui->projectNameEdit->text();

    _ui->createdPathLabel->setText(location);
}

void NewProjectWizard::accept()
{
    // Set up the new project
    _project = new Project(_ui->createdPathLabel->text());
    _project->initProject();

    QDialog::accept();
}
