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
#ifndef NODEITEM_HPP
#define NODEITEM_HPP

#include "graphitem.hpp"

namespace Developer {

class Node;

class NodeItem : public GraphItem
{
    Q_OBJECT

public:
    enum NodeShapes
    {
        Circle,
        Ellipse,
        Rectangle,
        RoundedRectangle
    };

    explicit NodeItem(Node *node, QGraphicsItem *parent = 0);
    explicit NodeItem(const QString &nodeId,
                      const QString &nodeLabel = QString(),
                      bool root = false,
                      QGraphicsItem *parent = 0);

    void recalculate();

    bool isRoot() const;
    bool marked() const;
    Node *node() const;

    void setId(const QString &itemId);
    void setLabel(const QString &itemLabel);
    void setIsRoot(bool root);
    void setMarked(bool isMarked);

    void preserveNode();
    void deleteNode();

    QPainterPath shape() const;
    QList<QPointF> intersection(QLineF line) const;

    QPointF centerPos() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    QColor backgroundColor(const QStyleOptionGraphicsItem *option) const;
    QColor borderColor(const QStyleOptionGraphicsItem *option) const;

public slots:
    void addedEdge();

protected slots:
    void positionChanged();

signals:
    void edgeAdded();
    void shapeChanged();

protected:
    // Handle hover events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    // Handle mouse presses
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    Node *_node;
    NodeShapes _nodeShape;
    bool _isRoot;
    bool _hover;
    bool _marked;
    QPainterPath _shape;
    QRectF _boundingRect;
};

}

#endif // NODEITEM_HPP
