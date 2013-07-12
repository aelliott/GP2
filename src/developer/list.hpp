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
#ifndef LIST_HPP
#define LIST_HPP

#include <QVariant>
#include <QString>

#include "parsertypes.hpp"

namespace Developer {

enum GPTypes
{
    //GP_Atom,
    GP_String,
    GP_Integer,
    GP_Variable
};

class Atom
{
public:
    Atom(const QString &str = QString());
    Atom(int intVal);

    GPTypes type() const;
    QString toString() const;
    int toInt() const;

    void setString(const QString &str);
    void setInt(int intVal);

private:
    GPTypes _type;
    QString _stringValue;
    int _intValue;
};

class ListValue
{
public:
    ListValue(const QString &variableID);
    ListValue(const Atom &atom = Atom());

    GPTypes type() const;
    QString variable() const;
    QString toString() const;
    int toInt() const;

    void setVariable(const QString &variableID);
    void setString(const QString &str);
    void setInt(int intVal);

private:
    GPTypes _type;
    QString _variableID;
    Atom _atom;
};

class List : public QVector<ListValue>
{
public:
    List();
    List(const QString &labelStr);
    List(label_t list);

    QString toString() const;
    label_t toLabel() const;
    QStringList variables() const;

    /*!
     * \brief Indicates whether the value passed in to the constructor was
     *  "clean"
     *
     * Clean inputs parsed without ignoring any characters, this is the default
     * and an empty list or one constructed by other means will be considered
     * clean.
     */
    bool isClean() const;

private:
    bool _clean;
};

}

#endif // LIST_HPP
