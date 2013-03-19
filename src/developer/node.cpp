/*!
 * \file
 */
#include "node.hpp"

namespace Developer {

Node::Node(const QString &nodeLabel)
    : _label(nodeLabel)
{
}

QString Node::label() const
{
    return _label;
}

void Node::setLabel(const QString &nodeLabel)
{
    _label = nodeLabel;
}

}
