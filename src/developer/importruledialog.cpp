/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "importruledialog.hpp"
#include "ui_importruledialog.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "project.hpp"

namespace Developer {

ImportRuleDialog::ImportRuleDialog(Project *project, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::ImportRuleDialog)
    , _project(project)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/helpdialog.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);
}

ImportRuleDialog::~ImportRuleDialog()
{
    delete _ui;
}

void ImportRuleDialog::selectFile()
{
    QString file = _ui->ruleFileEdit->text();
    QFileInfo info(file);

    QString dir;
    if(info.exists())
        dir = info.dir().path();
    else
        dir = QDir::homePath();

    file = QFileDialog::getOpenFileName(
                this,
                tr("Import Rule File"),
                dir,
                tr("GP Rule Files (*.gpr)"));

    if(!file.isEmpty())
        _ui->ruleFileEdit->setText(file);
}

void ImportRuleDialog::accept()
{
    QString path = _ui->ruleFileEdit->text();
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
        QDir dir = _project->rulesDir();
        QString newPath = dir.filePath(info.fileName());

        int i = 1;
        while(QFile(newPath).exists())
        {
            newPath = dir.filePath(info.baseName() + "_" +
                                   QVariant(i).toString() + "." + info.suffix());
            ++i;
        }

        QFile file(path);
        if(file.copy(newPath))
        {
            _project->addRule(newPath);
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
        _project->addRule(path);
        QDialog::accept();
    }
}

}
