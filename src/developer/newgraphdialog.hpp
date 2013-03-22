/*!
 * \file
 */
#ifndef NEWGRAPHDIALOG_HPP
#define NEWGRAPHDIALOG_HPP

#include <QDialog>

namespace Ui {
class NewGraphDialog;
}

namespace Developer {

class Project;

class NewGraphDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewGraphDialog(Project *proj, QWidget *parent = 0);
    ~NewGraphDialog();
    
private:
    Ui::NewGraphDialog *_ui;
    Project *_project;
};

}

#endif // NEWGRAPHDIALOG_HPP
