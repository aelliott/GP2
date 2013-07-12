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
#ifndef OPENPROJECTPROGRESSDIALOG_HPP
#define OPENPROJECTPROGRESSDIALOG_HPP

#include <QDialog>
#include <QThread>

namespace Ui {
class OpenProjectProgressDialog;
}

namespace Developer {

class Project;

class OpenProjectProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenProjectProgressDialog(Project *project = 0, QWidget *parent = 0);
    ~OpenProjectProgressDialog();

public slots:
    void setGraphFiles();
    void setRuleFiles();
    void setProgramFiles();

    void setNodes(int count);
    void setEdges(int count);

    void projectOpened();
    
private:
    Ui::OpenProjectProgressDialog *_ui;
    QThread *_thread;
    Project *_project;
};

}

#endif // OPENPROJECTPROGRESSDIALOG_HPP
