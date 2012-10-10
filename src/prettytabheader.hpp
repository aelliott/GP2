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
#ifndef PRETTYTABHEADER_HPP
#define PRETTYTABHEADER_HPP

#include <QWidget>

namespace Ui {
    class PrettyTabHeader;
}

/*!
 * \brief The PrettyTabHeader class is a simple form class which represents the
 *        PrettyTabBar's menu headings
 *
 * It is essentially just a QLabel contained within a pair of widgets to provide
 * the correct margins and background.
 */
class PrettyTabHeader : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrettyTabHeader(const QString &title, QWidget *parent = 0);
    ~PrettyTabHeader();
    
private:
    Ui::PrettyTabHeader *_ui;
};

#endif // PRETTYTABHEADER_HPP
