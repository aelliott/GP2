/*!
 * \file
 */
#ifndef NEWRULEDIALOG_HPP
#define NEWRULEDIALOG_HPP

#include <QDialog>

namespace Ui {
class NewRuleDialog;
}

namespace Developer {

class NewRuleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewRuleDialog(QWidget *parent = 0);
    ~NewRuleDialog();
    
private:
    Ui::NewRuleDialog *_ui;
};

}

#endif // NEWRULEDIALOG_HPP
