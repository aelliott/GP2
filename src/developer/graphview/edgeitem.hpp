/*!
 * \file
 */
#ifndef EDGEITEM_HPP
#define EDGEITEM_HPP

#include "nodeitem.hpp"

namespace Developer {

class Edge;

/*!
 * \brief The EdgeItem class provides a graphical representation of a graph Edge
 *  within the graph visualisation
 */
class EdgeItem : public GraphItem
{
    Q_OBJECT
public:
    explicit EdgeItem(Edge *edge, NodeItem *edgeFrom, NodeItem *edgeTo,
                      QGraphicsItem *parent = 0);
    explicit EdgeItem(const QString &edgeId, NodeItem *edgeFrom,
                      NodeItem *edgeTo, const QString &edgeLabel = QString(),
                      QGraphicsItem *parent = 0);

    NodeItem *from() const;
    NodeItem *to() const;

    void setFrom(NodeItem *edgeFrom);
    void setTo(NodeItem *edgeTo);

    QLineF line() const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    NodeItem *_from;
    NodeItem *_to;
};

}

#endif // EDGEITEM_HPP
