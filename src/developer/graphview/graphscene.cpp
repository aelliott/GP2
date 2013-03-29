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
    , _previousNode(0)
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
    n->setLabel(QString("n") + QVariant(static_cast<int>(_graph->nodes().size())
                                        ).toString());

    NodeItem *nodeItem = new NodeItem(n, 0);
    addItem(nodeItem);
    QRectF boundingRect = nodeItem->boundingRect();
    QPointF centerPos(position.x() - boundingRect.width()/2,
                      position.y() - boundingRect.height()/2
                      );
    nodeItem->setPos(centerPos);
    _nodes.insert(nodeItem->id(), nodeItem);

    // TEST CODE BEGINS
    if(_previousNode != 0)
    {
        Edge *e = _graph->addEdge(_graph->node(_previousNode->id()), n);
        EdgeItem *edgeItem = new EdgeItem(e, nodeItem, _previousNode);
        addItem(edgeItem);
        update(sceneRect());
    }

    _previousNode = nodeItem;
    // TEST CODE ENDS
}

void GraphScene::addNode(qreal x, qreal y)
{
    addNode(QPointF(x,y));
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    for(nodeIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
    {
        NodeItem *node = *iter;
        QPainterPath path = node->shape();
        path.translate(node->scenePos());
        if(path.contains(event->scenePos()))
        {
            QGraphicsScene::mouseDoubleClickEvent(event);
            return;
        }
    }

    event->accept();
    addNode(event->scenePos());
}

}
