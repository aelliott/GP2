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
#include "ruleedit.hpp"
#include "ui_ruleedit.h"

#include "rule.hpp"
#include "helpdialog.hpp"
#include "graph.hpp"

namespace Developer {

RuleEdit::RuleEdit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::RuleEdit)
    , _rule(0)
{
    _ui->setupUi(this);
}

RuleEdit::~RuleEdit()
{
    delete _ui;
}

void RuleEdit::setRule(Rule *rule)
{
    _rule = rule;

    _ui->nameEdit->setText(_rule->name());
    _ui->documentationEdit->setPlainText(_rule->documentation());
    _ui->rhsGraph->setLinkedGraph(0);
    _ui->lhsGraph->setGraph(_rule->lhs());
    connect(_rule->lhs(), SIGNAL(graphChanged()),
            this, SLOT(lhsChanged()));
    _ui->rhsGraph->setGraph(_rule->rhs());
    connect(_rule->rhs(), SIGNAL(graphChanged()),
            this, SLOT(rhsChanged()));
    _ui->rhsGraph->setLinkedGraph(_rule->lhs());
    _ui->conditionsEdit->setPlainText(_rule->condition());

    updateVariables();
}

void RuleEdit::showInjectiveHelp()
{
    HelpDialog *dialog = new HelpDialog(HelpDialog::Injective, this);
    dialog->exec();
}

void RuleEdit::nameChanged(QString name)
{
    _rule->setName(name);
}

void RuleEdit::documentationChanged()
{
    _rule->setDocumentation(_ui->documentationEdit->toPlainText());
}

void RuleEdit::lhsChanged()
{
    updateVariables();
}

void RuleEdit::rhsChanged()
{
    updateVariables();
}

void RuleEdit::injectiveChanged(int index)
{
    switch(index)
    {
    case 1:
        break;
    default:
        break;
    }
}

void RuleEdit::conditionChanged()
{
    if(_rule == 0)
        return;

    _ui->conditionsEdit->parse();
}

void RuleEdit::updateVariables()
{
    _ui->variablesWidget->clear();

    QStringList headers;
    headers << "Identifier" << "Type";
    _ui->variablesWidget->setHorizontalHeaderLabels(headers);

    QStringList lhsVariables = _rule->lhs()->variables();
    QStringList rhsVariables = _rule->rhs()->variables();

    QStringList variables = lhsVariables;
    variables += rhsVariables;
    variables.removeDuplicates();
    _ui->variablesWidget->setRowCount(variables.length());

    // Remove all the variables from the LHS from the RHS set, those that
    // remain are currently errors as they do not exist in the LHS
    QStringList diff = rhsVariables;
    for(int i = 0; i < lhsVariables.length(); ++i)
    {
        QString variable = lhsVariables.at(i);
        if(diff.contains(variable))
            diff.removeOne(variable);
    }

    QStringList types;
    types << "List" << "Atom" << "String" << "Integer";
    for(int i = 0; i < variables.length(); ++i)
    {
        QString variable = variables.at(i);
        QTableWidgetItem *item = new QTableWidgetItem(variable);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        if(diff.contains(variable))
        {
            item->setToolTip(tr("The variable '%1' does not appear in the LHS. "
                                "All variables in the RHS must appear in the "
                                "LHS graph.").arg(variable));
            item->setBackgroundColor(QColor(0xff,0xaa,0xaa,0x55));
        }

        _ui->variablesWidget->setItem(i, 0, item);

        QComboBox *comboBox = new QComboBox(_ui->variablesWidget);
        comboBox->addItems(types);
        _ui->variablesWidget->setCellWidget(i, 1, comboBox);
    }
}

}
