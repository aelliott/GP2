/*!
 * \file
 */
#ifndef EDITNODEDIALOG_HPP
#define EDITNODEDIALOG_HPP

#include <QDialog>

namespace Ui {
class EditNodeDialog;
}

namespace Developer {

class EditNodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditNodeDialog(QWidget *parent = 0);
    ~EditNodeDialog();
    
private:
    Ui::EditNodeDialog *_ui;
};

}

#endif // EDITNODEDIALOG_HPP
