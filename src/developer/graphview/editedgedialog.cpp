/*!
 * \file
 */
#include "editedgedialog.hpp"
#include "ui_editedgedialog.h"

#include <QFile>

namespace Developer {

EditEdgeDialog::EditEdgeDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::EditEdgeDialog)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);
}

EditEdgeDialog::~EditEdgeDialog()
{
    delete _ui;
}

}
