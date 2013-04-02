/*!
 * \file
 */
#include "graphscene.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <QPainter>
#include <QSettings>

namespace Developer {

GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent)
    , _internalGraph(true)
    , _drawingEdge(false)
{
    _graph = new Graph();
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundBrush(QColor(Qt::white));
}

Graph *GraphScene::graph() const
{
    return _graph;
}

void GraphScene::setGraph(Graph *newGraph)
{
    // Remove child items from the scene
    qDeleteAll(items());
    _nodes.clear();
    _edges.clear();

    // Only delete if this is an internal graph being replaced
    if(_internalGraph)
        delete _graph;

    _graph = newGraph;
    _internalGraph = false;

    QRect canvas = _graph->canvas();
    if(!canvas.isNull())
    {
        // Sanity checks
        Q_ASSERT(canvas.width()  > 0);
        Q_ASSERT(canvas.height() > 0);

        setSceneRect(canvas);
    }

    std::vector<Node *> nList = _graph->nodes();
    for(std::vector<Node *>::iterator iter = nList.begin(); iter != nList.end();
        ++iter)
    {
        Node *n = *iter;

        Q_ASSERT(!_nodes.contains(n->id()));

        NodeItem *nodeItem = new NodeItem(n);
        addNodeItem(nodeItem, n->pos());
    }

    std::vector<Edge *> eList = _graph->edges();
    for(std::vector<Edge *>::iterator iter = eList.begin(); iter != eList.end();
        ++iter)
    {
        Edge *e = *iter;

        Q_ASSERT(!_edges.contains(e->id()));
        NodeItem *from;
        NodeItem *to;

        if(!_nodes.contains(e->from()->id()))
        {
            qDebug() << "Edge missing node with ID " << e->from()->id();
            qDebug() << "Ignoring.";
            continue;
        }
        from = _nodes[e->from()->id()];

        if(!_nodes.contains(e->to()->id()))
        {
            qDebug() << "Edge missing node with ID " << e->from()->id();
            qDebug() << "Ignoring.";
            continue;
        }
        to = _nodes[e->to()->id()];

        EdgeItem *edgeItem = new EdgeItem(e, from, to);
        addEdgeItem(edgeItem);
    }
}

void GraphScene::addNodeItem(NodeItem *nodeItem, const QPointF &position)
{
    addItem(nodeItem);
    nodeItem->setPos(position);
    _nodes.insert(nodeItem->id(), nodeItem);
    emit nodeAdded(nodeItem);
}

void GraphScene::addEdgeItem(EdgeItem *edgeItem)
{
    addItem(edgeItem);
    _edges.insert(edgeItem->id(), edgeItem);
    emit edgeAdded(edgeItem);
}

void GraphScene::addNode(const QPointF &position)
{
    Node *n = _graph->addNode();
    n->setLabel(QString("n") + QVariant(static_cast<int>(_graph->nodes().size())
                                        ).toString());

    NodeItem *nodeItem = new NodeItem(n, 0);
    QRectF boundingRect = nodeItem->boundingRect();
    QPointF centerPos(position.x() - boundingRect.width()/2,
                      position.y() - boundingRect.height()/2
                      );
    addNodeItem(nodeItem, centerPos);
}

void GraphScene::addNode(qreal x, qreal y)
{
    addNode(QPointF(x,y));
}

void GraphScene::nodeIdChanged(QString oldId, QString newId)
{
    if(_nodes.contains(oldId))
    {
        NodeItem *nodeItem = _nodes[oldId];
        _nodes.remove(oldId);
        _nodes.insert(newId, nodeItem);
    }
}

void GraphScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    QSettings settings;
    qreal arrowSize = settings.value("GraphView/Edges/ArrowSize", 9).toDouble();
    qreal lineWidth = settings.value("GraphView/Edges/LineWidth", 1.5).toDouble();
    QColor lineColour  = settings.value("GraphView/Edges/LineColour",
                                        QColor(0x33,0x33,0x33)).value<QColor>();

    if(_drawingEdge)
    {
        QLineF initial(_fromNode->centerPos(), _mousePos);
        QList<QPointF> intersections = _fromNode->intersection(initial);

        if(intersections.size() < 1)
        {
            // This occurs whilst the mouse pointer is still inside the node in
            // question. We can start drawing when it is outside.
            return;
        }

        QLineF edgeLine = initial;
        edgeLine.setP1(intersections.at(0));
        QLineF drawLine = edgeLine;
        // Compensate for pen width
        drawLine.setLength(drawLine.length()-(lineWidth+0.5));
        QPainterPath painterPath(drawLine.p1());
        painterPath.lineTo(drawLine.p2());

        // Abuse QLineF's ability to transform lines in order to draw a triangle
        // at the end of this edge as follows:

        // Get the current angle
        qreal angle = drawLine.angle();
        // Shift it around by 150 degrees, this leaves 30 degrees as the internal
        // angle which is what we want as the edge cuts the equilateral triangle's
        // 60 degree internal angles in half
        // If we overshoot and get negative simply add 360 to fix it
        angle -= 150; if(angle < 0) angle += 360;
        // Move the line such that p1 is now where p2 sat
        drawLine.translate(drawLine.p2()-drawLine.p1());
        // Resize the line to be the length of an edge of the triangle
        drawLine.setLength(arrowSize);
        // Set the new angle, p2 is now in the correct place
        drawLine.setAngle(angle);
        // Draw a line to this point
        painterPath.lineTo(drawLine.p2());

        // Now that we've done the initial line the rest are all relative to the
        // equilateral triangle, and therefore we are moving to produce 60 degree
        // internal angles
        angle -= 120; if(angle < 0) angle += 360;
        // The rest is exactly the same
        drawLine.translate(drawLine.p2()-drawLine.p1());
        drawLine.setLength(arrowSize);
        drawLine.setAngle(angle);
        painterPath.lineTo(drawLine.p2());

        // One last time to get us back to our origin
        angle -= 120; if(angle < 0) angle += 360;
        drawLine.translate(drawLine.p2()-drawLine.p1());
        drawLine.setLength(arrowSize);
        drawLine.setAngle(angle);
        painterPath.lineTo(drawLine.p2());

        QPen edgePen(lineColour);
        edgePen.setWidth(lineWidth);
        painter->setPen(edgePen);
        painter->setBrush(lineColour);
        painter->drawPath(painterPath);
    }
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        // Are we over a node?
        for(nodeIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
        {
            NodeItem *node = *iter;
            QPainterPath path = node->shape();
            path.translate(node->scenePos());
            if(path.contains(event->scenePos()))
            {
                _drawingEdge = true;
                _fromNode = node;
                _mousePos = event->scenePos();
                update();
                return;
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    _mousePos = event->scenePos();
    QGraphicsScene::mouseMoveEvent(event);

    QList<QGraphicsItem *> atPoint = items(event->scenePos());
    for(int i = 1; i < atPoint.length(); ++i)
    {
        QGraphicsSceneHoverEvent *hoverEvent = new QGraphicsSceneHoverEvent(
                    QEvent::GraphicsSceneHoverMove);
        hoverEvent->setScenePos(event->scenePos());
        sendEvent(atPoint.at(i), hoverEvent);
    }
    update();
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(_drawingEdge)
    {
        _drawingEdge = false;
        // Are we above a node at this point? If yes we need to add an edge
        for(nodeIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
        {
            NodeItem *node = *iter;
            QPainterPath path = node->shape();
            path.translate(node->scenePos());
            if(path.contains(event->scenePos()))
            {
                QString newLabel = _fromNode->label() + "_" + node->label();
                Node *from = _graph->node(_fromNode->id());
                Node *to = _graph->node(node->id());
                if(from == 0 || to == 0)
                {
                    qDebug() << "Edge creation failed to find nodes.";
                    return;
                }
                Edge *edge = _graph->addEdge(from, to, newLabel);
                EdgeItem *edgeItem = new EdgeItem(edge, _fromNode, node);
                addEdgeItem(edgeItem);
                return;
            }
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
    update();
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // If we're over a node then don't add a new node, instead pass the event on
    // to trigger an EditNodeDialog
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

    // And the same for edges
    for(edgeIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        EdgeItem *edge = *iter;
        if(edge->edgePolygon().containsPoint(event->scenePos(), Qt::OddEvenFill))
        {
            QGraphicsScene::mouseDoubleClickEvent(event);
            return;
        }
    }

    event->accept();
    addNode(event->scenePos());
}

}
