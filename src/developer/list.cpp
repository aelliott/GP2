/*!
 * \file
 */
#include "list.hpp"

#include <QRegExp>
#include <QStringList>

namespace Developer {

Atom::Atom(const QString &str)
    : _type(GP_String)
    , _stringValue(str)
    , _intValue(-1)
{
}

Atom::Atom(int intVal)
    : _type(GP_Integer)
    , _stringValue()
    , _intValue(intVal)
{
}

GPTypes Atom::type() const
{
    return _type;
}

QString Atom::toString() const
{
    if(type() == GP_String)
        return _stringValue;
    else
        return QString("\"") + QVariant(_intValue).toString() + "\"";
}

int Atom::toInt() const
{
    if(type() == GP_Integer)
        return _intValue;
    else
        return QVariant(_stringValue).toInt();
}

void Atom::setString(const QString &str)
{
    _stringValue = str;
    _type = GP_String;
}

void Atom::setInt(int intVal)
{
    _intValue = intVal;
    _type = GP_Integer;
}

ListValue::ListValue(const Atom &atom)
    : _type(atom.type())
    , _variableID()
    , _atom(atom)
{
}

ListValue::ListValue(const QString &variableID)
    : _type(GP_Variable)
    , _variableID(variableID)
    , _atom()
{
}

GPTypes ListValue::type() const
{
    return _type;
}

QString ListValue::variable() const
{
    // Special case
    if(_variableID == "empty")
        return QString();

    return _variableID;
}

QString ListValue::toString() const
{
    return _atom.toString();
}

int ListValue::toInt() const
{
    return _atom.toInt();
}

void ListValue::setVariable(const QString &variableID)
{
    _variableID = variableID;
    _type = GP_Variable;
}

void ListValue::setString(const QString &str)
{
    _atom.setString(str);
    _type = GP_String;
}

void ListValue::setInt(int intVal)
{
    _atom.setInt(intVal);
    _type = GP_Integer;
}

List::List()
    : QVector()
{
}

List::List(const QString &labelStr)
{
    int labelPos = 0;
    bool needsValue = true;
    QRegExp rx;
    while(labelPos < labelStr.length())
    {
        // Check if we want a value or if we're expecting either an end of
        // string or a list value separator (:)
        if(!needsValue)
        {
            if(labelStr.at(labelPos) == QChar(':'))
            {
                needsValue = true;
                ++labelPos;
            }
            else
            {
                qDebug() << "Unexpected char " << labelStr.at(labelPos) << " at "
                         << "position " << labelPos << ", was expecting a list "
                         << "value separator (:).";
                ++labelPos;
                continue;
            }
        }
        else if(labelStr.at(labelPos) == QChar(':'))
        {
            // If we encounter one right after another value then add an empty
            ++labelPos;
            push_back(ListValue("empty"));
            continue;
        }

        rx = QRegExp("\"[^\"]*\"");
        if(rx.indexIn(labelStr, labelPos) == labelPos)
        {
            labelPos += rx.matchedLength();
            // Empty strings should be marked as such
            if(rx.matchedLength() == 2)
            {
                push_back(ListValue("empty"));
                needsValue = false;
                continue;
            }
            QString str = rx.cap(0);
            str.remove("\"");
            push_back(ListValue(Atom(str)));
            needsValue = false;
            continue;
        }

        rx = QRegExp("\\d+");
        if(rx.indexIn(labelStr, labelPos) == labelPos)
        {
            labelPos += rx.matchedLength();
            QVariant num = rx.cap(0);
            int value = num.toInt();
            push_back(ListValue(Atom(value)));
            needsValue = false;
            continue;
        }

        rx = QRegExp("[a-zA-Z_][a-zA-Z0-9_]{,62}");
        if(rx.indexIn(labelStr, labelPos) == labelPos)
        {
            labelPos += rx.matchedLength();
            QString identifier = rx.cap(0);
            push_back(ListValue(identifier));
            needsValue = false;
            continue;
        }

        qDebug() << "Unexpected char " << labelStr.at(labelPos) << " at  position "
                 << labelPos << ", was expecting an atom or variable.";
        ++labelPos;
    }
}

List::List(label_t label)
{
    for(std::vector<atom_t>::iterator iter = label.values.begin();
        iter != label.values.end(); ++iter)
    {
        atom_t atom = *iter;
        std::string *str = boost::get<std::string>(&atom);
        int *intVal = boost::get<int>(&atom);

        if(str)
        {
            if(str->length() == 0)
                push_back(ListValue("empty"));
            else
            {
                if(str->at(0) == '"')
                    push_back(ListValue(Atom(str->c_str())));
                else
                    push_back(ListValue(str->c_str()));
            }
        }
        else if(intVal)
            push_back(ListValue(Atom(*intVal)));
        else
            qDebug() << "Variant containing no value passed to List()";
    }
}

QString List::toString() const
{
    QString result = "";

    bool first = true;
    for(List::const_iterator iter = begin(); iter != end(); ++iter)
    {
        if(first)
            first = false;
        else
            result += ":";

        ListValue val = *iter;
        switch(val.type())
        {
        case GP_String:
            result += val.toString();
            break;
        case GP_Integer:
            result += QVariant(val.toInt()).toString();
            break;
        case GP_Variable:
            result += val.variable();
            break;
        }
    }

    return result;
}

label_t List::toLabel() const
{
    label_t label;

    for(List::const_iterator iter = begin(); iter != end(); ++iter)
    {
        ListValue value = *iter;

        switch(value.type())
        {
        case GP_String:
            label.values.push_back(value.toString().toStdString());
            break;
        case GP_Integer:
            label.values.push_back(value.toInt());
            break;
        case GP_Variable:
            label.values.push_back(value.variable().toStdString());
            break;
        }
    }

    return label;
}

QStringList List::variables() const
{
    QStringList result;

    for(List::const_iterator iter = begin(); iter != end(); ++iter)
    {
        ListValue value = *iter;

        if(value.type() == GP_Variable)
            result << value.variable();
    }

    return result;
}

}
