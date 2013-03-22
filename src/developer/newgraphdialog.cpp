/*!
 * \file
 */
#include "newgraphdialog.hpp"
#include "ui_newgraphdialog.h"

#include "project.hpp"

namespace Developer {

NewGraphDialog::NewGraphDialog(Project *proj, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::NewGraphDialog)
    , _project(proj)
{
    _ui->setupUi(this);
}

NewGraphDialog::~NewGraphDialog()
{
    delete _ui;
}

}
