/*!
 * Copyright (c) 2012 Alex Elliott <alex@alex-elliott.co.uk>
 *
 * This file is part of SlackTracker
 *
 * SlackTracker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Expression editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Expression editor. If not, see <http://www.gnu.org/licenses/>.
 */
#include "prettytabheader.hpp"
#include "ui_prettytabheader.h"

PrettyTabHeader::PrettyTabHeader(const QString &title, QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::PrettyTabHeader)
{
    _ui->setupUi(this);
    _ui->label->setText(title);
}

PrettyTabHeader::~PrettyTabHeader()
{
    delete _ui;
}
