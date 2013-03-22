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

class NewGraphDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewGraphDialog(QWidget *parent = 0);
    ~NewGraphDialog();
    
private:
    Ui::NewGraphDialog *_ui;
};

}

#endif // NEWGRAPHDIALOG_HPP
