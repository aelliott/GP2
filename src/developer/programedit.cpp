/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "programedit.hpp"
#include "ui_programedit.h"

#include "program.hpp"

#include <QDebug>
#include <QSettings>
#include <QFont>

namespace Developer {

ProgramEdit::ProgramEdit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::ProgramEdit)
    , _program(0)
    , _programCache("")
    , _documentationCache("")
    , _setUp(false)
{
    _ui->setupUi(this);
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

    _setUp = false;
    _program = program;
    _programCache = _program->program();
    _documentationCache = _program->documentation();
    _ui->documentationEdit->setPlainText(_program->documentation());
    _ui->editor->setPlainText(_program->program());
    _ui->editor->parse();
    _setUp = true;

     _ui->documentationEdit->setEnabled(_program->status() != GPFile::ReadOnly);
     _ui->editor->setEnabled(_program->status() != GPFile::ReadOnly);
}

void ProgramEdit::textEdited()
{
    if(!_setUp)
        return;

    QString prog = _ui->editor->toPlainText();
    QString docs = _ui->documentationEdit->toPlainText();
    if(_program != 0
            && _programCache != prog)
    {
        _ui->editor->parse();
        _programCache = prog;
        _program->setProgram(prog);
    }
    else if(_program != 0
            && _documentationCache != docs)
    {
        _programCache = docs;
        _program->setDocumentation(docs);
    }
}

}
