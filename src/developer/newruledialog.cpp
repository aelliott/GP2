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
#include "newruledialog.hpp"
#include "ui_newruledialog.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "project.hpp"


namespace Developer {

NewRuleDialog::NewRuleDialog(Project *proj, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::NewRuleDialog)
    , _project(proj)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/helpdialog.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    // Initialise form
    // Set a default rule name as "Rule"
    _ui->ruleNameEdit->setText("Rule");

    // Set the default directory as "${projectDir}/rules"
    QDir dir = proj->dir();
    dir.cd("rules");
    _ui->ruleDirEdit->setText(QDir::toNativeSeparators(dir.path()));

    // Update the path field
    updatePath();
}

NewRuleDialog::~NewRuleDialog()
{
    delete _ui;
}

void NewRuleDialog::nameChanged(QString name)
{
    Q_UNUSED(name)
    updatePath();
}

void NewRuleDialog::selectDir()
{
    QDir dir(_ui->ruleDirEdit->text());
    if(!dir.exists())
        dir = _project->dir();
    QString newDir = QFileDialog::getExistingDirectory(
                this,
                tr("Select Directory"),
                dir.path()
                );
    if(!newDir.isEmpty())
    {
        _ui->ruleDirEdit->setText(newDir);
        updatePath();
    }
}

void NewRuleDialog::dirChanged(QString dir)
{
    Q_UNUSED(dir)
    updatePath();
}

void NewRuleDialog::updatePath()
{
    QString fileName = _ui->ruleNameEdit->text() + GP_RULE_EXTENSION;
    QDir dir(_ui->ruleDirEdit->text());
    _ui->rulePath->setText(dir.filePath(fileName));
}

void NewRuleDialog::accept()
{
    // Ignore any empty inputs
    if(_ui->ruleNameEdit->text().isEmpty()
            || _ui->ruleDirEdit->text().isEmpty())
        return;

    // Check if the directory we're targetting exists, if not offer to create it
    QDir dir(_ui->ruleDirEdit->text());
    if(!dir.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, tr("Create Directory?"),
                                      tr("The directory specified (%1) does not"
                                         " exist, create it?").arg(dir.path()),
                                      QMessageBox::Yes | QMessageBox::Cancel
                                      );
        if(reply != QMessageBox::Yes)
            return;

        dir.mkpath(dir.path());
    }

    QString rule = dir.filePath(_ui->ruleNameEdit->text() + GP_RULE_EXTENSION);
    _project->newRule(rule);

    QDialog::accept();
}

}
