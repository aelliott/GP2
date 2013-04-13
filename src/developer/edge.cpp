/*!
 * \file
 */
#include "edge.hpp"

namespace Developer {

Edge::Edge(const QString &edgeId, Node *fromNode, Node *toNode, const List &edgeLabel, Graph *parent)
    : _parent(parent)
    , _id(edgeId)
    , _from(fromNode)
    , _to(toNode)
    , _label(edgeLabel)
    , _phantom(false)
{
}

Edge::Edge(const QString &edgeId, Node *fromNode, Node *toNode, Graph *parent)
    : _parent(parent)
    , _id(edgeId)
    , _from(fromNode)
    , _to(toNode)
    , _label()
    , _phantom(false)
{
}

QString Edge::id() const
{
    return _id;
}

Node *Edge::from() const
{
    return _from;
}

Node *Edge::to() const
{
    return _to;
}

List Edge::label() const
{
    return _label;
}

Graph *Edge::parent() const
{
    return _parent;
}

bool Edge::isPhantomEdge() const
{
    return _phantom;
}

void Edge::setId(const QString &edgeId)
{
    _id = edgeId;
}

void Edge::setFrom(Node *fromNode)
{
    _from = fromNode;
}

void Edge::setTo(Node *toNode)
{
    _to = toNode;
}

void Edge::setLabel(const List &edgeLabel)
{
    _label = edgeLabel;
}

void Edge::setPhantom(bool phantom)
{
    _phantom = phantom;
}

}
