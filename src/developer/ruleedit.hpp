/*!
 * \file
 */
#ifndef RULEEDIT_HPP
#define RULEEDIT_HPP

#include <QWidget>

namespace Ui {
class RuleEdit;
}

namespace Developer {

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
    /*!
     * \brief Slot to handle displaying the "injective matching" help page from
     *  the HelpDialog
     */
    void showInjectiveHelp();

private:
    Ui::RuleEdit *_ui;
};

}

#endif // RULEEDIT_HPP
