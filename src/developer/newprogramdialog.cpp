/*!
 * \file
 */
#include "newprogramdialog.hpp"
#include "ui_newprogramdialog.h"

namespace Developer {

NewProgramDialog::NewProgramDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::NewProgramDialog)
{
    _ui->setupUi(this);
}

NewProgramDialog::~NewProgramDialog()
{
    delete _ui;
}

}
