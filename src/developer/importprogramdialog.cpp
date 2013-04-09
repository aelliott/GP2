/*!
 * \file
 */
#include "importprogramdialog.hpp"
#include "ui_importprogramdialog.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "project.hpp"

namespace Developer {

ImportProgramDialog::ImportProgramDialog(Project *project, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::ImportProgramDialog)
    , _project(project)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/helpdialog.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);
}

ImportProgramDialog::~ImportProgramDialog()
{
    delete _ui;
}

void ImportProgramDialog::selectFile()
{
    QString file = _ui->programFileEdit->text();
    QFileInfo info(file);

    QString dir;
    if(info.exists())
        dir = info.dir().path();
    else
        dir = QDir::homePath();

    file = QFileDialog::getOpenFileName(
                this,
                tr("Import Program File"),
                dir,
                tr("GP Program Files (*.gpx)"));

    if(!file.isEmpty())
        _ui->programFileEdit->setText(file);
}

void ImportProgramDialog::accept()
{
    QString path = _ui->programFileEdit->text();
    QFileInfo info(path);
    if(path.isEmpty() || !info.exists())
    {
        QMessageBox::warning(
                    this,
                    tr("Cannot Import File"),
                    tr("The file provided is invalid or does not exist, cannot "
                       "import the file into the project."));
        return;
    }

    if(_ui->copyCheckBox->isChecked())
    {
        QDir dir = _project->programsDir();
        QString newPath = dir.filePath(info.fileName());

        int i = 1;
        while(QFile(newPath).exists())
        {
            newPath = dir.filePath(info.baseName() + "_" +
                                   QVariant(i).toString() + info.suffix());
            ++i;
        }

        QFile file(path);
        if(file.copy(newPath))
        {
            _project->addProgram(newPath);
            QDialog::accept();
        }
        else
        {
            QMessageBox::warning(
                        this,
                        tr("Failed to Import File"),
                        tr("GP Developer could not copy the file at '%1' to "
                           "'%2' - unknown error, perhaps a file already exists "
                           "there").arg(path, newPath));
            QDialog::reject();
        }
    }
    else
    {
        _project->addProgram(path);
        QDialog::accept();
    }
}

}
