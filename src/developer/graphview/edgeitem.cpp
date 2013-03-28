/*!
 * \file
 */
#include "edgeitem.hpp"
#include "edge.hpp"

namespace Developer {

EdgeItem::EdgeItem(Edge *edge, NodeItem *edgeFrom, NodeItem *edgeTo,
                   QGraphicsItem *parent)
    : GraphItem(edge->id(), edge->label(), parent)
    , _from(edgeFrom)
    , _to(edgeTo)
{
}

EdgeItem::EdgeItem(const QString &edgeId, NodeItem *edgeFrom, NodeItem *edgeTo,
                   const QString &edgeLabel, QGraphicsItem *parent)
    : GraphItem(edgeId, edgeLabel,parent)
    , _from(edgeFrom)
    , _to(edgeTo)
{
}

NodeItem *EdgeItem::from() const
{
    return _from;
}

NodeItem *EdgeItem::to() const
{
    return _to;
}

void EdgeItem::setFrom(NodeItem *edgeFrom)
{
    _from = edgeFrom;
}

void EdgeItem::setTo(NodeItem *edgeTo)
{
    _to = edgeTo;
}

}
