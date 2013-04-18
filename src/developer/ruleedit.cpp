/*!
 * \file
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

    QStringList variables = _rule->lhs()->variables();
    variables += _rule->rhs()->variables();
    variables.removeDuplicates();
    _ui->variablesWidget->setRowCount(variables.length());

    QStringList types;
    types << "List" << "Atom" << "String" << "Integer";
    for(int i = 0; i < variables.length(); ++i)
    {
        QString variable = variables.at(i);
        QTableWidgetItem *item = new QTableWidgetItem(variable);
        item->setFlags(Qt::ItemIsSelectable);

        _ui->variablesWidget->setItem(i, 0, item);

        QComboBox *comboBox = new QComboBox(_ui->variablesWidget);
        comboBox->addItems(types);
        _ui->variablesWidget->setCellWidget(i, 1, comboBox);
    }
}

}
