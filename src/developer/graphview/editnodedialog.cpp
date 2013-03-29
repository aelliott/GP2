/*!
 * \file
 */
#include "editnodedialog.hpp"
#include "ui_editnodedialog.h"

#include <QFile>

namespace Developer {

EditNodeDialog::EditNodeDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::EditNodeDialog)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);
}

EditNodeDialog::~EditNodeDialog()
{
    delete _ui;
}

}
