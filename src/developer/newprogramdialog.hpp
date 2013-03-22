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

class Project;

class NewProgramDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewProgramDialog(Project *proj, QWidget *parent = 0);
    ~NewProgramDialog();
    
private:
    Ui::NewProgramDialog *_ui;
    Project *_project;
};

}

#endif // NEWPROGRAMDIALOG_HPP
