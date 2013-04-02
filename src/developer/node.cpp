/*!
 * \file
 */
#include "node.hpp"

#include <QRegExp>

namespace Developer {

Node::Node(const QString &nodeId, const QString &nodeLabel, const QPointF &nodePos)
    : _id(nodeId)
    , _label(nodeLabel)
    , _pos(nodePos)
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

QPointF Node::pos() const
{
    return _pos;
}

qreal Node::xPos() const
{
    return _pos.x();
}

qreal Node::yPos() const
{
    return _pos.y();
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

void Node::setPos(const QPointF &nodePos)
{
    _pos = nodePos;
}

void Node::setPos(qreal x, qreal y)
{
    setPos(QPointF(x, y));
}

void Node::setIsRoot(bool root)
{
    _isRoot = root;
}

}
