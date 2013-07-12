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
#include "styledbutton.hpp"
#include "ui_styledbutton.h"

#include <QMouseEvent>

namespace Developer {

StyledButton::StyledButton(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::StyledButton)
{
    _ui->setupUi(this);

    setStyleSheet("QLabel#subtextLabel { color: #555; }");
}

StyledButton::~StyledButton()
{
    delete _ui;
}

void StyledButton::setMainText(QString text)
{
    _ui->mainTextLabel->setText(text);
}

void StyledButton::setSubtext(QString text)
{
    _ui->subtextLabel->setText(text);
}

void StyledButton::enterEvent(QEvent *event)
{
    event->accept();
    setStyleSheet("QLabel#subtextLabel { color: #555; } QWidget#buttonBackground { background: #f0f0f0; }");
}

void StyledButton::leaveEvent(QEvent *event)
{
    event->accept();
    setStyleSheet("QLabel#subtextLabel { color: #555; }");
}

void StyledButton::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    emit pressed();
}

}
