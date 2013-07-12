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
#ifndef PROJECTPREFERENCES_HPP
#define PROJECTPREFERENCES_HPP

#include "preferencespage.hpp"

namespace Ui {
    class ProjectPreferences;
}

namespace Developer {

class ProjectPreferences : public PreferencesPage
{
    Q_OBJECT
    
public:
    explicit ProjectPreferences(QWidget *parent = 0);
    ~ProjectPreferences();

    void reset();
    void apply();

public slots:
    void selectDefaultProjectLocation();
    
private:
    Ui::ProjectPreferences *_ui;
};

}

#endif // PROJECTPREFERENCES_HPP
