/*!
 * \file
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

    bool isRoot() const;

    void setIsRoot(bool root);

    QPainterPath shape() const;
    QList<QPointF> intersection(QLineF line) const;

    QPointF centerPos() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected:
    // Handle hover events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    // Handle mouse presses
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    NodeShapes _shape;
    bool _isRoot;
    bool _hover;
};

}

#endif // NODEITEM_HPP
