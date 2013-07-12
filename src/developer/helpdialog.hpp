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
#ifndef HELPDIALOG_HPP
#define HELPDIALOG_HPP

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
    class HelpDialog;
}

namespace Developer {

class HelpDialog : public QDialog
{
    Q_OBJECT
    
public:
    /*!
     * Simple enum to make the process of inserting a new page into the help
     * dialog simpler, simply add a new enum value in the correct part of the
     * sequence and it will automatically map to the correct stackedWidget
     * index.
     */
    enum HelpPages
    {
        Introduction,
        Welcome,
        GeneralConcepts,
        GP,
        Rules,
        Programs,
        Injective
    };

    explicit HelpDialog(HelpPages initialPage = Introduction, QWidget *parent = 0);
    ~HelpDialog();

public slots:
    void helpItemClicked(QTreeWidgetItem *item);
    
private:
    Ui::HelpDialog *_ui;
};

}

#endif // HELPDIALOG_HPP
