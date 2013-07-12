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
#ifndef LISTVALIDATOR_HPP
#define LISTVALIDATOR_HPP

#include <QValidator>

namespace Developer {

class ListValidator : public QValidator
{
    Q_OBJECT
public:
    explicit ListValidator(QObject *parent = 0);

    State validate(QString &input, int &pos) const;
};

}

#endif // LISTVALIDATOR_HPP
