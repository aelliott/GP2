/*!
 * \file
 */
#include "programedit.hpp"
#include "ui_programedit.h"

#include "program.hpp"

#include <Qsci/qscilexercpp.h>

#include <QDebug>
#include <QSettings>
#include <QFont>

namespace Developer {

ProgramEdit::ProgramEdit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::ProgramEdit)
{
    _ui->setupUi(this);

    QSettings settings;
    QFont font = settings.value("Editor/Font", EDITOR_DEFAULT_FONT).value<QFont>();

    _ui->programEdit->setFont(font);
    _ui->programEdit->setIndentationGuides(true);
    _ui->programEdit->setLexer(new QsciLexerCPP(_ui->programEdit));
    _ui->programEdit->setMarginLineNumbers(1, true);
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
    _ui->programEdit->setText(program->program());
}

}
