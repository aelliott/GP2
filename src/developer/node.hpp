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
    Node(const QString &nodeId, const QString &nodeLabel = QString());

    QString id() const;
    QString label() const;
    bool isRoot() const;

    void setId(const QString &nodeId);
    void setLabel(const QString &nodeLabel);
    void setIsRoot(bool root);

private:
    QString _id;
    QString _label;
    bool _isRoot;
};

}

#endif // NODE_HPP
