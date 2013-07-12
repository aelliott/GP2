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
#ifndef RULEEDIT_HPP
#define RULEEDIT_HPP

#include <QWidget>

namespace Ui {
class RuleEdit;
}

namespace Developer {

class Rule;

/*!
 * \brief The RuleEdit class encapsulates the UI components for editing rules
 */
class RuleEdit : public QWidget
{
    Q_OBJECT
    
public:
    /*!
     * \brief Construct a new RuleEdit
     *
     * The widget will not be ready for use immediately since this construction
     * will occur at application start-up, and typically a project has not been
     * selected at this stage.
     *
     * Before the user is able to reach this widget the system should ensure
     * that setRule() has been called with a valid rule - the Edit class should
     * guarantee this via its Edit::setProject() member function.
     *
     * \param parent    This widget's parent widget (Edit)
     */
    explicit RuleEdit(QWidget *parent = 0);
    /*!
     * \brief Destroy this widget and free memory
     */
    ~RuleEdit();

public slots:
    void setRule(Rule *rule);

    void nameChanged(QString name);
    void documentationChanged();
    void lhsChanged();
    void rhsChanged();
    void injectiveChanged(int index);
    void conditionChanged();

    void updateVariables();

    /*!
     * \brief Slot to handle displaying the "injective matching" help page from
     *  the HelpDialog
     */
    void showInjectiveHelp();

private:
    Ui::RuleEdit *_ui;
    Rule *_rule;
};

}

#endif // RULEEDIT_HPP
