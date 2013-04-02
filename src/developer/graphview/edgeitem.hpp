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

    QPolygonF polygon() const;
    QPolygonF edgePolygon(double padding = 4.0) const;
    QRectF boundingRect() const;
    QPainterPath path() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

public slots:
    void nodeMoved();

protected:
    // Handle hover events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    // Handle mouse movement
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    // Handle mouse button events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    NodeItem *_from;
    NodeItem *_to;
    bool _hover;
    bool _fromAnchor;
    bool _toAnchor;
};

}

#endif // EDGEITEM_HPP
