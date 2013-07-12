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
#ifndef QUICKRUNWIDGET_HPP
#define QUICKRUNWIDGET_HPP

#include <QWidget>

namespace Ui {
class QuickRunWidget;
}

namespace Developer {

class Project;

/*!
 * \brief The QuickRunWidget class allows the user to quickly run a saved run
 *  configuration
 *
 * This class sits in the top right of the MainWindow and allow the user to
 * select one of their defined run configurations from a combo box and then run
 * them using an adjacent button.
 */
class QuickRunWidget : public QWidget
{
    Q_OBJECT
    
public:
    /*!
     * \brief Create a new QuickRunWidget
     * \param parent This widget's parent widget (MainWindow)
     */
    explicit QuickRunWidget(QWidget *parent = 0);
    /*!
     * \brief Destroy this QuickRunWidget and free memory
     */
    ~QuickRunWidget();

    /*!
     * \brief Set the currently open project to the provided project
     * \param project   The new project which run configurations will be drawn
     *  from
     */
    void setProject(Project *project);

public slots:
    /*!
     * \brief Slot which handles the user pressing the "run" button
     *
     * Whichever run configuration currently appears in the widget's combo box
     * should be run.
     */
    void run();
    
private:
    Ui::QuickRunWidget *_ui;
    Project *_project;
};

}

#endif // QUICKRUNWIDGET_HPP
