/*!
 * \file
 */
#include "graphscene.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

namespace Developer {

GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent)
    , _internalGraph(true)
{
    _graph = new Graph();
}

Graph *GraphScene::graph() const
{
    return _graph;
}

void GraphScene::setGraph(Graph *newGraph)
{
    // Remove child items from the scene
    qDeleteAll(items());

    // Only delete if this is an internal graph being replaced
    if(_internalGraph)
        delete _graph;

    _graph = newGraph;
    _internalGraph = false;
}

void GraphScene::addNode(const QPointF &position)
{
    Node *n = _graph->addNode();
    n->setLabel(QString("n") + n->id());

    NodeItem *nodeItem = new NodeItem(n, 0);
    addItem(nodeItem);
    QRectF boundingRect = nodeItem->boundingRect();
    QPointF centerPos(position.x() - boundingRect.width()/2,
                      position.y() - boundingRect.height()/2
                      );
    nodeItem->setPos(centerPos);
}

void GraphScene::addNode(qreal x, qreal y)
{
    addNode(QPointF(x,y));
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();

    addNode(event->scenePos());
}

}
