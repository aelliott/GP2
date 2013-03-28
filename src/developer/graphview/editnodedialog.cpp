/*!
 * \file
 */
#include "editnodedialog.hpp"
#include "ui_editnodedialog.h"

namespace Developer {

EditNodeDialog::EditNodeDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::EditNodeDialog)
{
    _ui->setupUi(this);
}

EditNodeDialog::~EditNodeDialog()
{
    delete _ui;
}

}
