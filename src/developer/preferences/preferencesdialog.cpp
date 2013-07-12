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
#include "preferencesdialog.hpp"
#include "ui_preferencesdialog.h"

#include <QFile>
#include <QAbstractButton>

#include "projectpreferences.hpp"
#include "appearancepreferences.hpp"
#include "toolchainpreferences.hpp"

namespace Developer {

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::PreferencesDialog)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/preferences.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    ProjectPreferences *proj = new ProjectPreferences(this);
    _pages.push_back(proj);

    AppearancePreferences *app = new AppearancePreferences(this);
    _pages.push_back(app);

    ToolchainPreferences *toolchains = new ToolchainPreferences(this);
    _pages.push_back(toolchains);

    _ui->mainWidget->addTab(proj,
                            QIcon(QPixmap(":/icons/folder.png")),
                            tr("Projects")
                            );

    _ui->mainWidget->addTab(app,
                            QIcon(QPixmap(":/icons/palette.png")),
                            tr("Appearance")
                            );

    _ui->mainWidget->addTab(toolchains,
                            QIcon(QPixmap(":/icons/cog.png")),
                            tr("GP Toolchains")
                            );
}

PreferencesDialog::~PreferencesDialog()
{
    delete _ui;
}

void PreferencesDialog::reset()
{
    pageIter iter;
    for(iter = _pages.begin(); iter != _pages.end(); ++iter)
        (*iter)->reset();
}

void PreferencesDialog::apply()
{
    pageIter iter;
    for(iter = _pages.begin(); iter != _pages.end(); ++iter)
        (*iter)->apply();
}

void PreferencesDialog::buttonClicked(QAbstractButton *button)
{
    if(button->text() == QString("OK"))
        accept();
    if(button->text() == QString("Cancel"))
        reject();
    if(button->text() == QString("Reset"))
        reset();
    if(button->text() == QString("Apply"))
        apply();
}

void PreferencesDialog::accept()
{
    apply();
    QDialog::accept();
}

void PreferencesDialog::reject()
{
    QDialog::reject();
}

}
