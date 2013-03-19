/*!
 * \file
 */
#include "ruleedit.hpp"
#include "ui_ruleedit.h"

#include "helpdialog.hpp"

namespace Developer {

RuleEdit::RuleEdit(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::RuleEdit)
{
    _ui->setupUi(this);
}

RuleEdit::~RuleEdit()
{
    delete _ui;
}

void RuleEdit::showInjectiveHelp()
{
    HelpDialog *dialog = new HelpDialog(HelpDialog::Injective, this);
    dialog->exec();
}

}
