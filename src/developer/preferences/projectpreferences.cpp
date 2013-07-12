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
#include "projectpreferences.hpp"
#include "ui_projectpreferences.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

namespace Developer {

ProjectPreferences::ProjectPreferences(QWidget *parent)
    : PreferencesPage(parent)
    , _ui(new Ui::ProjectPreferences)
{
    _ui->setupUi(this);

    reset();
}

ProjectPreferences::~ProjectPreferences()
{
    delete _ui;
}

void ProjectPreferences::reset()
{
    _ui->defaultProjectLocationEdit->setText(
                _settings.value(
                    "Projects/DefaultProjectLocation",
                    QVariant(QDir::toNativeSeparators(
                                 QDir::homePath()
                                 )
                             ).toString()
                    ).toString()
                );
}

void ProjectPreferences::apply()
{
    QDir dir;

    if(dir.exists(_ui->defaultProjectLocationEdit->text()))
    {
        _settings.setValue(
                    "Projects/DefaultProjectLocation",
                    _ui->defaultProjectLocationEdit->text()
                    );
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, tr("Create Directory?"),
                                      tr("The directory specified (%1) does not"
                                         " exist, create it?").arg(_ui->defaultProjectLocationEdit->text()),
                                      QMessageBox::Yes | QMessageBox::Cancel
                                      );

        if(reply == QMessageBox::Yes)
        {
            if(!dir.mkpath(_ui->defaultProjectLocationEdit->text()))
            {
                QMessageBox::warning(
                            this,
                            tr("Directory Creation Failed"),
                            tr("Could not create the specified path")
                            );
            }
            else
            {
                if(dir.exists(_ui->defaultProjectLocationEdit->text()))
                {
                    _settings.setValue(
                                "Projects/DefaultProjectLocation",
                                _ui->defaultProjectLocationEdit->text()
                                );
                }
            }
        }
    }
}

void ProjectPreferences::selectDefaultProjectLocation()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Select Default Project Directory"),
                _ui->defaultProjectLocationEdit->text()
                );
    dir = QDir::toNativeSeparators(dir);

    if(!dir.isEmpty())
        _ui->defaultProjectLocationEdit->setText(dir);
}

}
