/*!
 * \file
 */
#ifndef NODE_HPP
#define NODE_HPP

#include <QString>

namespace Developer {

/*!
 * \brief The Node class represents a node within a GP graph
 */
class Node
{
public:
    Node(const QString &nodeLabel = QString());

    QString label() const;

    void setLabel(const QString &nodeLabel);

private:
    QString _label;
};

}

#endif // NODE_HPP
