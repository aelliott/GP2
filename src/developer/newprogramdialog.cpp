/*!
 * \file
 */
#include "newprogramdialog.hpp"
#include "ui_newprogramdialog.h"

#include "project.hpp"

namespace Developer {

NewProgramDialog::NewProgramDialog(Project *proj, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::NewProgramDialog)
    , _project(proj)
{
    _ui->setupUi(this);
}

NewProgramDialog::~NewProgramDialog()
{
    delete _ui;
}

}
