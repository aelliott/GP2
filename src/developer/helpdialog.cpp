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
#include "helpdialog.hpp"
#include "ui_helpdialog.h"

#include <QFile>
#include <QDebug>

namespace Developer {

HelpDialog::HelpDialog(HelpPages initialPage, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::HelpDialog)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/helpdialog.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->stackedWidget->setCurrentIndex(initialPage);
    if(initialPage == Injective)
        _ui->title->setText(tr("Help - ") + tr("Injective Matching"));
}

HelpDialog::~HelpDialog()
{
    delete _ui;
}

void HelpDialog::helpItemClicked(QTreeWidgetItem *item)
{
    _ui->title->setText(tr("Help - ") + item->text(0));

    if(item->text(0) == tr("Introduction"))
    {
        _ui->stackedWidget->setCurrentIndex(Introduction);
        return;
    }

    if(item->text(0) == tr("Welcome"))
    {
        _ui->stackedWidget->setCurrentIndex(Welcome);
        return;
    }

    if(item->text(0) == tr("General Concepts"))
    {
        _ui->stackedWidget->setCurrentIndex(GeneralConcepts);
        return;
    }

    qDebug() << "Help page not found for " << item->text(0);
}

}
