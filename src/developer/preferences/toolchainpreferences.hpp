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
#ifndef TOOLCHAINPREFERENCES_HPP
#define TOOLCHAINPREFERENCES_HPP

#include "preferencespage.hpp"

namespace Ui {
class ToolchainPreferences;
}

namespace Developer {

class ToolchainPreferences : public PreferencesPage
{
    Q_OBJECT
    
public:
    explicit ToolchainPreferences(QWidget *parent = 0);
    ~ToolchainPreferences();

    void reset();
    void apply();
    
private:
    Ui::ToolchainPreferences *_ui;
};

}

#endif // TOOLCHAINPREFERENCES_HPP
