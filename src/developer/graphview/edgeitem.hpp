/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
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

    Edge *edge() const;
    NodeItem *from() const;
    NodeItem *to() const;

    void setFrom(NodeItem *edgeFrom);
    void setTo(NodeItem *edgeTo);

    void preserveEdge();
    void deleteEdge();

    QLineF line() const;

    QPolygonF polygon(double polygonWidth = -1.0) const;
    QPolygonF edgePolygon(double padding = 4.0) const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QPainterPath path() const;
    QPainterPath arrowHead(qreal adjustment = 0.0) const;
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
    Edge *_edge;
    QRectF _boundingRect;
    QMap<qreal, QPolygonF> _polygons;
    QPainterPath _shape;
    QPainterPath _path;
    QMap<qreal, QPainterPath> _arrowHeads;
    NodeItem *_from;
    NodeItem *_to;
    bool _hover;
};

}

#endif // EDGEITEM_HPP
