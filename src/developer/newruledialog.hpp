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
#ifndef NEWRULEDIALOG_HPP
#define NEWRULEDIALOG_HPP

#include <QDialog>

namespace Ui {
class NewRuleDialog;
}

namespace Developer {

class Project;

class NewRuleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewRuleDialog(Project *proj, QWidget *parent = 0);
    ~NewRuleDialog();

public slots:
    void nameChanged(QString name);
    void selectDir();
    void dirChanged(QString dir);

    void updatePath();

    void accept();
    
private:
    Ui::NewRuleDialog *_ui;
    Project *_project;
};

}

#endif // NEWRULEDIALOG_HPP
