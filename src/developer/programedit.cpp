/*!
 * \file
 */
#include "programedit.hpp"
#include "ui_programedit.h"

#include "program.hpp"
#include "programhighlighter.hpp"

#include <QDebug>
#include <QSettings>
#include <QFont>

namespace Developer {

ProgramEdit::ProgramEdit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::ProgramEdit)
    , _program(0)
{
    _ui->setupUi(this);

    QSettings settings;
    QFont font = settings.value("Editor/Font", EDITOR_DEFAULT_FONT).value<QFont>();
    _ui->editor->setFont(font);
    _highlighter = new ProgramHighlighter(_ui->editor->document());
}

ProgramEdit::~ProgramEdit()
{
    delete _ui;
}

void ProgramEdit::setProgram(Program *program)
{
    if(program == 0)
    {
        qDebug() << "ProgramEdit::setProgram() received a null pointer. Ignoring.";
        return;
    }

    _program = program;
    _ui->editor->setPlainText(_program->program());
}

void ProgramEdit::textEdited()
{
    if(_program != 0)
        _program->setProgram(_ui->editor->toPlainText());
}

}
