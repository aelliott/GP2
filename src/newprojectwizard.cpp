#include "newprojectwizard.hpp"
#include "ui_newprojectwizard.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QRegExp>

NewProjectWizard::NewProjectWizard(QWidget *parent)
    : QWizard(parent)
    , _ui(new Ui::NewProjectWizard)
    , _project(0)
    , _fileNameValidation("^(\\w|\\d|\\-|_|\\.)*$")
{
    _ui->setupUi(this);

#ifndef Q_OS_WIN32
    // The Linux and OSX styles do not fit in with the rest of the application,
    // however the Aero style looks pretty good with the existing stylesheet.
    setWizardStyle(QWizard::ModernStyle);
#endif // Q_OS_WIN32

    QSettings settings;
    QString path = settings.value("Projects/DefaultProjectLocation", QVariant(QDir::toNativeSeparators(QDir::homePath()))).toString();

    _ui->projectLocationEdit->setText(path);
    _ui->createdPathLabel->setText(path);

    _ui->projectNameEdit->setValidator(new QRegExpValidator(_fileNameValidation, this));
    _ui->initialRuleEdit->setValidator(new QRegExpValidator(_fileNameValidation, this));
    _ui->initialProgramEdit->setValidator(new QRegExpValidator(_fileNameValidation, this));
    _ui->initialGraphEdit->setValidator(new QRegExpValidator(_fileNameValidation, this));
}

NewProjectWizard::~NewProjectWizard()
{
    delete _ui;
}

Project *NewProjectWizard::project() const
{
    return _project;
}

bool NewProjectWizard::validateCurrentPage()
{
    switch(currentId())
    {
        case 0:
            // Validate the target directory
        {
            QDir dir;
            if(!dir.exists(_ui->projectLocationEdit->text()))
                return false;
        }
            // Validate the project name
            if(_ui->projectNameEdit->text() == QString()
                    || !_fileNameValidation.exactMatch(_ui->projectNameEdit->text())
                    )
                return false;
            break;
        case 1:
            // Validate initial rule/program/graph names
            if(!_fileNameValidation.exactMatch(_ui->initialRuleEdit->text())
                    || !_fileNameValidation.exactMatch(_ui->initialProgramEdit->text())
                    || !_fileNameValidation.exactMatch(_ui->initialGraphEdit->text())
                    )
                return false;
            break;
        default:
            // If given no other information, return true by default
            return true;
    }

    // No reason to fail found, accept the data
    return true;
}

void NewProjectWizard::selectProjectLocation()
{
    QFileDialog fileDialog(this);
    QString dir = fileDialog.getExistingDirectory(this, tr("Select a Project Directory"), _ui->projectLocationEdit->text());
    _ui->projectLocationEdit->setText(QDir::toNativeSeparators(dir));
}

void NewProjectWizard::updateProjectLocation()
{
    _projectPath = QDir::toNativeSeparators(_ui->projectLocationEdit->text());
    if(!_projectPath.endsWith(QDir::separator()))
        _projectPath += QDir::separator();

    _projectPath += _ui->projectNameEdit->text();

    _ui->createdPathLabel->setText(_projectPath);
}

void NewProjectWizard::accept()
{
    // Check if the default project location should be updated (even if it's
    // with the same value for simplicity's sake)
    if(_ui->updateDefaultLocationCheckBox->isChecked())
    {
        QSettings settings;
        settings.setValue(
                    "Projects/DefaultProjectLocation",
                    _ui->projectLocationEdit->text()
                    );
    }

    // Set up the new project
    _project = new Project();
    _project->initProject(_projectPath, _ui->projectNameEdit->text());

    QDialog::accept();
}
