/*!
 * \file
 */
#ifndef EDITEDGEDIALOG_HPP
#define EDITEDGEDIALOG_HPP

#include <QDialog>

namespace Ui {
class EditEdgeDialog;
}

namespace Developer {

class EditEdgeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditEdgeDialog(QWidget *parent = 0);
    ~EditEdgeDialog();
    
private:
    Ui::EditEdgeDialog *_ui;
};

}

#endif // EDITEDGEDIALOG_HPP
