/*!
 * \file
 */
#include "programedit.hpp"
#include "ui_programedit.h"

namespace Developer {

ProgramEdit::ProgramEdit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::ProgramEdit)
{
    _ui->setupUi(this);
}

ProgramEdit::~ProgramEdit()
{
    delete _ui;
}

}
