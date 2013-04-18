/*!
 * \file
 */
#include "editnodedialog.hpp"
#include "ui_editnodedialog.h"

#include "nodeitem.hpp"
#include "listvalidator.hpp"

#include <QFile>

namespace Developer {

EditNodeDialog::EditNodeDialog(NodeItem *node, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::EditNodeDialog)
    , _node(node)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->idEdit->setText(node->id());
    _ui->labelEdit->setText(node->label());
    _ui->rootCheckBox->setChecked(node->isRoot());
    _ui->markedCheckBox->setChecked(node->marked());

    _labelValidator = new ListValidator(this);
    _ui->labelEdit->setValidator(_labelValidator);
}

EditNodeDialog::~EditNodeDialog()
{
    delete _ui;
    delete _labelValidator;
}

void EditNodeDialog::accept()
{
    _node->setId(_ui->idEdit->text());
    _node->setLabel(_ui->labelEdit->text());
    _node->setIsRoot(_ui->rootCheckBox->isChecked());
    _node->setMarked(_ui->markedCheckBox->isChecked());

    QDialog::accept();
}

}
