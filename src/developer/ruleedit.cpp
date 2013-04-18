/*!
 * \file
 */
#include "ruleedit.hpp"
#include "ui_ruleedit.h"

#include "rule.hpp"
#include "helpdialog.hpp"

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
    _ui->rhsGraph->setGraph(_rule->rhs());
    _ui->rhsGraph->setLinkedGraph(_rule->lhs());
    _ui->conditionsEdit->setPlainText(_rule->condition());
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

}

void RuleEdit::rhsChanged()
{

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

}
