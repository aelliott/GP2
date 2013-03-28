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

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    NodeShapes _shape;
    bool _isRoot;
};

}

#endif // NODEITEM_HPP
