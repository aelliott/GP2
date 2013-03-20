/*!
 * \file
 */
#ifndef PROGRAMEDIT_HPP
#define PROGRAMEDIT_HPP

#include <QWidget>

namespace Ui {
class ProgramEdit;
}

namespace Developer {

class ProgramEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgramEdit(QWidget *parent = 0);
    ~ProgramEdit();
    
private:
    Ui::ProgramEdit *_ui;
};

}

#endif // PROGRAMEDIT_HPP
