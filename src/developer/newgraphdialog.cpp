/*!
 * \file
 */
#include "newgraphdialog.hpp"
#include "ui_newgraphdialog.h"

namespace Developer {

NewGraphDialog::NewGraphDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::NewGraphDialog)
{
    _ui->setupUi(this);
}

NewGraphDialog::~NewGraphDialog()
{
    delete _ui;
}

}
