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

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/helpdialog.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);
}

NewGraphDialog::~NewGraphDialog()
{
    delete _ui;
}

}
