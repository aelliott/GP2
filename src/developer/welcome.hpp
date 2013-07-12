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
#ifndef WELCOME_HPP
#define WELCOME_HPP

#include <QWidget>
#include <QSignalMapper>

namespace Ui {
    class Welcome;
}

namespace Developer {

class Welcome : public QWidget
{
    Q_OBJECT
    
public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();

public slots:
    void recentProjectsUpdated(QStringList projects);

signals:
    void newProjectClicked();
    void openProjectClicked();
    void openProjectClicked(QString);

private slots:
    void newProject();
    void openProject(QString path = QString());
    
private:
    Ui::Welcome *_ui;
    QSignalMapper *_mapper;
};

}

#endif // WELCOME_HPP
