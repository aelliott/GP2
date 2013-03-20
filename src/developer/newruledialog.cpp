/*!
 * \file
 */
#include "newruledialog.hpp"
#include "ui_newruledialog.h"

#include <QFile>

namespace Developer {

NewRuleDialog::NewRuleDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::NewRuleDialog)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/helpdialog.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);
}

NewRuleDialog::~NewRuleDialog()
{
    delete _ui;
}

}
