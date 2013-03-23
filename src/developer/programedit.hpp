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
class ProgramHighlighter;

class ProgramEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgramEdit(QWidget *parent = 0);
    ~ProgramEdit();

    void setProgram(Program *program);

public slots:
    void textEdited();
    
private:
    Ui::ProgramEdit *_ui;
    Program *_program;
    ProgramHighlighter *_highlighter;
};

}

#endif // PROGRAMEDIT_HPP
