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

class Program;

class ProgramEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgramEdit(QWidget *parent = 0);
    ~ProgramEdit();

    void setProgram(Program *program);
    
private:
    Ui::ProgramEdit *_ui;
    Program *_program;
};

}

#endif // PROGRAMEDIT_HPP
