/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
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

    //_labelValidator = new ListValidator(this);
    //_ui->labelEdit->setValidator(_labelValidator);
}

EditEdgeDialog::~EditEdgeDialog()
{
    delete _ui;
    //delete _labelValidator;
}

void EditEdgeDialog::accept()
{
    _edge->setId(_ui->idEdit->text());
    _edge->setLabel(_ui->labelEdit->text());

    _edge->update();
    QDialog::accept();
}

}
