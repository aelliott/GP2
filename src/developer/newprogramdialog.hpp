/*!
 * \file
 */
#ifndef NEWPROGRAMDIALOG_HPP
#define NEWPROGRAMDIALOG_HPP

#include <QDialog>

namespace Ui {
class NewProgramDialog;
}

namespace Developer {

class NewProgramDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewProgramDialog(QWidget *parent = 0);
    ~NewProgramDialog();
    
private:
    Ui::NewProgramDialog *_ui;
};

}

#endif // NEWPROGRAMDIALOG_HPP
