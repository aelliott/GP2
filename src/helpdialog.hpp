#ifndef HELPDIALOG_HPP
#define HELPDIALOG_HPP

#include <QDialog>

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();
    
private:
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_HPP
