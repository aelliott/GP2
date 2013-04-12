/*!
 * \file
 */
#include "edge.hpp"

namespace Developer {

Edge::Edge(const QString &edgeId, Node *fromNode, Node *toNode, const List &edgeLabel)
    : _id(edgeId)
    , _from(fromNode)
    , _to(toNode)
    , _label(edgeLabel)
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

}
