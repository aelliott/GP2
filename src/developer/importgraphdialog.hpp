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
#ifndef IMPORTGRAPHDIALOG_HPP
#define IMPORTGRAPHDIALOG_HPP

#include <QDialog>

namespace Ui {
class ImportGraphDialog;
}

namespace Developer {

class Project;

class ImportGraphDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImportGraphDialog(Project *project = 0, QWidget *parent = 0);
    ~ImportGraphDialog();

public slots:
    void selectFile();
    void accept();
    
private:
    Ui::ImportGraphDialog *_ui;
    Project *_project;
};

}

#endif // IMPORTGRAPHDIALOG_HPP
