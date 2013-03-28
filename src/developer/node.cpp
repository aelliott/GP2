/*!
 * \file
 */
#include "node.hpp"

#include <QRegExp>

namespace Developer {

Node::Node(const QString &nodeId, const QString &nodeLabel)
    : _id(nodeId)
    , _label(nodeLabel)
    , _isRoot(false)
{
    if(_id.endsWith("(R)"))
    {
        _isRoot = true;
        _id.remove(QRegExp("\\((r|R)\\)$"));
    }
}

QString Node::id() const
{
    return _id;
}

QString Node::label() const
{
    return _label;
}

bool Node::isRoot() const
{
    return _isRoot;
}

void Node::setId(const QString &nodeId)
{
    _id = nodeId;
}

void Node::setLabel(const QString &nodeLabel)
{
    _label = nodeLabel;
}

void Node::setIsRoot(bool root)
{
    _isRoot = root;
}

}
