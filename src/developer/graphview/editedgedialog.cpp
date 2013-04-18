/*!
 * \file
 */
#include "editedgedialog.hpp"
#include "ui_editedgedialog.h"

#include "graphview/edgeitem.hpp"
#include "listvalidator.hpp"
#include "edge.hpp"
#include "graph.hpp"

#include <QFile>

namespace Developer {

EditEdgeDialog::EditEdgeDialog(EdgeItem *edgeItem, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::EditEdgeDialog)
    , _edge(edgeItem)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    QStringList nodes = _edge->edge()->parent()->nodeIdentifiers();

    _ui->idEdit->setText(_edge->id());
    _ui->labelEdit->setText(_edge->label());

    _ui->fromComboBox->addItems(nodes);
    _ui->toComboBox->addItems(nodes);

    _ui->fromComboBox->setCurrentIndex(
                _ui->fromComboBox->findText(_edge->from()->id())
                );
    _ui->toComboBox->setCurrentIndex(
                _ui->fromComboBox->findText(_edge->to()->id())
                );

    _labelValidator = new ListValidator(this);
    _ui->labelEdit->setValidator(_labelValidator);
}

EditEdgeDialog::~EditEdgeDialog()
{
    delete _ui;
}

}
