/*!
 * \file
 */
#include "node.hpp"
#include "graph.hpp"

#include <QRegExp>

namespace Developer {

Node::Node(const QString &nodeId, const List &nodeLabel, const QPointF &nodePos, Graph *parent)
    : _id(nodeId)
    , _label(nodeLabel)
    , _pos(nodePos)
    , _isRoot(false)
    , _parent(parent)
{
    if(_id.endsWith("(R)"))
    {
        _isRoot = true;
        _id.remove(QRegExp("\\((r|R)\\)$"));
    }
}

Node::Node(const QString &nodeId, Graph *parent)
{
    Node(nodeId, List(), QPointF(), parent);
}

QString Node::id() const
{
    return _id;
}

List Node::label() const
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

std::vector<Edge *> Node::edges() const
{
    return _parent->edges(id());
}

std::vector<Edge *> Node::edgesFrom() const
{
    return _parent->edgesFrom(id());
}

std::vector<Edge *> Node::edgesTo() const
{
    return _parent->edgesTo(id());
}

bool Node::hasEdgeOut() const
{
    return (_parent->edgeFrom(id()) != 0);
}

bool Node::hasEdgeIn() const
{
    return (_parent->edgeTo(id()) != 0);
}

Graph *Node::parent() const
{
    return _parent;
}

void Node::setId(const QString &nodeId)
{
    _id = nodeId;
}

void Node::setLabel(const List &nodeLabel)
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
