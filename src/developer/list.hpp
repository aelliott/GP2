/*!
 * \file
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
};

}

#endif // LIST_HPP
