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
#ifndef NEWPROJECTWIZARD_HPP
#define NEWPROJECTWIZARD_HPP

#include <QWizard>
#include "project.hpp"

namespace Ui {
    class NewProjectWizard;
}

namespace Developer {

class NewProjectWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit NewProjectWizard(QWidget *parent = 0);
    ~NewProjectWizard();

    Project *project() const;

    bool validateCurrentPage();

public slots:
    void selectProjectLocation();
    void updateProjectLocation();
    void accept();
    
private:
    Ui::NewProjectWizard *_ui;
    QString _projectPath;
    Project *_project;
    QRegExp _fileNameValidation;
};

}

#endif // NEWPROJECTWIZARD_HPP
