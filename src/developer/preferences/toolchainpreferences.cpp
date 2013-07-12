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
#include "toolchainpreferences.hpp"
#include "ui_toolchainpreferences.h"

namespace Developer {

ToolchainPreferences::ToolchainPreferences(QWidget *parent)
    : PreferencesPage(parent)
    , _ui(new Ui::ToolchainPreferences)
{
    _ui->setupUi(this);
}

ToolchainPreferences::~ToolchainPreferences()
{
    delete _ui;
}

void ToolchainPreferences::reset()
{
    QSettings settings;
}

void ToolchainPreferences::apply()
{
    QSettings settings;
}

}
