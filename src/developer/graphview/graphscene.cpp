/*!
 * \file
 */
#include "graphscene.hpp"

#include <ogdf/tree/TreeLayout.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/tree/RadialTreeLayout.h>
#include <ogdf/planarlayout/FPPLayout.h>
#include <ogdf/planarlayout/PlanarDrawLayout.h>
#include <ogdf/planarlayout/PlanarStraightLayout.h>
#include <ogdf/planarlayout/SchnyderLayout.h>
#include <ogdf/planarity/PlanarizationGridLayout.h>
#include <ogdf/layered/FastHierarchyLayout.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/DavidsonHarelLayout.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/GEMLayout.h>

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

#include <QPainter>
#include <QSettings>

namespace Developer {

using ogdf::GraphAttributes;

GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent)
    , _internalGraph(true)
    , _drawingEdge(false)
    , _selecting(false)
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

    bool layoutSet = false;
    std::vector<Node *> nList = _graph->nodes();
    for(std::vector<Node *>::iterator iter = nList.begin(); iter != nList.end();
        ++iter)
    {
        Node *n = *iter;

        Q_ASSERT(!_nodes.contains(n->id()));

        NodeItem *nodeItem = new NodeItem(n);
        addNodeItem(nodeItem, n->pos());

        layoutSet = (nodeItem->pos().x() != 0 || nodeItem->pos().y() != 0);
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

    if(!layoutSet)
        layoutCircular();

    resizeToContents();
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

void GraphScene::layoutInit()
{
    _g = ogdf::Graph();
    _ga = GraphAttributes(_g, GraphAttributes::nodeGraphics
                       | GraphAttributes::edgeGraphics
                       | GraphAttributes::nodeLabel
                       | GraphAttributes::edgeLabel);

    _nodeMap.clear();

    for(nodeIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
    {
        NodeItem *node = *iter;
        ogdf::node ogdfNode = _g.newNode();
        _ga.x(ogdfNode) = node->pos().x();
        _ga.y(ogdfNode) = node->pos().y();
        _ga.labelNode(ogdfNode) = ogdf::String(node->label().toStdString().c_str());
        _ga.width(ogdfNode) = node->shape().boundingRect().width();
        _ga.height(ogdfNode) = node->shape().boundingRect().height();
        _nodeMap.insert(node->id(), ogdfNode);
    }

    for(edgeIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        EdgeItem *edge = *iter;
        Q_ASSERT(_nodeMap.contains(edge->from()->id()));
        Q_ASSERT(_nodeMap.contains(edge->to()->id()));
        ogdf::edge ogdfEdge = _g.newEdge(
                    _nodeMap[edge->from()->id()],
                    _nodeMap[edge->to()->id()]
                );
        _ga.labelEdge(ogdfEdge) = ogdf::String(edge->label().toStdString().c_str());
    }
}

void GraphScene::layoutTree(LayoutDirections direction)
{
    layoutInit();

    ogdf::TreeLayout tree;
    tree.siblingDistance(40.0);
    tree.subtreeDistance(30.0);
    tree.levelDistance(60.0);
    tree.treeDistance(60.0);

    switch(direction)
    {
    case Layout_RightToLeft:
        tree.orientation(ogdf::rightToLeft);
        break;
    case Layout_BottomToTop:
        tree.orientation(ogdf::bottomToTop);
        break;
    case Layout_LeftToRight:
        tree.orientation(ogdf::leftToRight);
        break;
    case Layout_TopToBottom:
    default:
        tree.orientation(ogdf::topToBottom);
        break;
    }

    tree.call(_ga);

    layoutApply();
}

void GraphScene::layoutSugiyama()
{
    layoutInit();

    ogdf::SugiyamaLayout sugiyama;

    ogdf::FastHierarchyLayout *fhl = new ogdf::FastHierarchyLayout;
    fhl->layerDistance(45.0);
    fhl->nodeDistance(30.0);
    sugiyama.setLayout(fhl);

    sugiyama.call(_ga);

    layoutApply();
}

void GraphScene::layoutRadialTree()
{
    layoutInit();

    ogdf::RadialTreeLayout radialTree;
    radialTree.levelDistance(60.0);
    radialTree.connectedComponentDistance(60.0);

    radialTree.call(_ga);

    layoutApply();
}

void GraphScene::layoutFPP()
{
    layoutInit();

    ogdf::FPPLayout fpp;

    fpp.call(_ga);

    layoutApply();
}

void GraphScene::layoutPlanarDraw()
{
    layoutInit();

    ogdf::PlanarDrawLayout planarDraw;

    planarDraw.call(_ga);

    layoutApply();
}

void GraphScene::layoutPlanarStraight()
{
    layoutInit();

    ogdf::PlanarStraightLayout planarStraight;

    planarStraight.call(_ga);

    layoutApply();
}

void GraphScene::layoutSchnyder()
{
    layoutInit();

    ogdf::SchnyderLayout schnyder;

    schnyder.call(_ga);

    layoutApply();
}

void GraphScene::layoutPlanarizationGrid()
{
    layoutInit();

    ogdf::PlanarizationGridLayout planarGrid;
    planarGrid.separation(50.0);

    planarGrid.call(_ga);

    layoutApply();
}

void GraphScene::layoutCircular()
{
    layoutInit();

    ogdf::CircularLayout circular;
    circular.minDistCircle(50.0);
    circular.minDistLevel(50.0);
    circular.minDistSibling(35.0);
    circular.minDistCC(50.0);

    circular.call(_ga);

    layoutApply();
}

void GraphScene::layoutSpring()
{
    layoutInit();

    ogdf::SpringEmbedderFR spring;
    spring.minDistCC(40.0);
    spring.scaleFunctionFactor(5.0);

    spring.call(_ga);

    layoutApply();
}

void GraphScene::layoutDavidsonHarel()
{
    layoutInit();

    ogdf::DavidsonHarelLayout dh;

    dh.call(_ga);

    layoutApply();
}

void GraphScene::layoutFMMM()
{
    layoutInit();

    ogdf::FMMMLayout fmmm;

    fmmm.call(_ga);

    layoutApply();
}

void GraphScene::layoutGEM()
{
    layoutInit();

    ogdf::GEMLayout gem;

    gem.call(_ga);
    gem.desiredLength(20.0);
    gem.minDistCC(50.0);

    layoutApply();
}

void GraphScene::layoutApply()
{
    for(nodeIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
    {
        NodeItem *node = *iter;
        QPointF newPosition;
        newPosition.setX(_ga.x(_nodeMap[node->id()]));
        newPosition.setY(_ga.y(_nodeMap[node->id()]));
        node->setPos(newPosition);
    }

    resizeToContents();
}

void GraphScene::resizeToContents()
{
    QRectF boundingRect = itemsBoundingRect();
    QPointF topLeft = boundingRect.topLeft();
    QPointF bottomRight = boundingRect.bottomRight();

    topLeft.setX(topLeft.x() - 20.0);
    topLeft.setY(topLeft.y() - 20.0);

    bottomRight.setX(bottomRight.x() + 20.0);
    bottomRight.setY(bottomRight.y() + 20.0);

    boundingRect = QRectF(topLeft, bottomRight);
    setSceneRect(boundingRect);
    _graph->setCanvas(boundingRect.toRect());
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
    Q_UNUSED(rect)
    QSettings settings;
    qreal arrowSize = settings.value("GraphView/Edges/ArrowSize", 9).toDouble();
    qreal lineWidth = settings.value("GraphView/Edges/LineWidth", 1.5).toDouble();
    QColor lineColour  = settings.value("GraphView/Edges/LineColour",
                                        QColor(0x33,0x33,0x33)).value<QColor>();
    lineColour.setAlpha(100);

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

    if(_selecting)
    {
        QColor selectionColour(0xff,0xff,0xcc, 160);
        QColor selectionBorderColour(0xff, 0xff, 0x99, 255);

        painter->setPen(selectionBorderColour);
        painter->setBrush(selectionColour);

        painter->drawRect(QRectF(_mouseInitialPos, _mousePos));

        QPainterPath path;
        path.addRect(QRectF(_mouseInitialPos, _mousePos));

        setSelectionArea(path);
    }
}

EdgeItem *GraphScene::edgeItem(const QString &id) const
{
    for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        EdgeItem *edge = *iter;
        if(edge->id() == id)
            return edge;
    }

    return 0;
}

void GraphScene::removeEdge(EdgeItem *edge)
{
    if(!_graph->removeEdge(edge->id()))
        return;
    edgeIter iter = _edges.begin();
    while(iter != _edges.end()
          && (*iter)->id() != edge->id())
        ++iter;
    _edges.remove(iter.key());
    removeItem(edge);
    delete edge;
}

void GraphScene::removeNode(NodeItem *node)
{
    std::vector<Edge *> edges = node->node()->edges();
    for(std::vector<Edge *>::iterator iter = edges.begin();
        iter != edges.end(); ++iter)
    {
        Edge *edge = *iter;
        removeEdge(edgeItem(edge->id()));
    }

    if(!_graph->removeNode(node->id()))
        return;

    nodeIter iter = _nodes.begin();
    while(iter != _nodes.end()
          && (*iter)->id() != node->id())
        ++iter;
    _nodes.remove(iter.key());
    removeItem(node);
}

void GraphScene::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Delete:
    {
        QList<QGraphicsItem *> selected = selectedItems();
        if(selected.count() > 0)
        {
            for(QList<QGraphicsItem *>::iterator iter = selected.begin();
                iter != selected.end(); ++iter)
            {
                QGraphicsItem *item = *iter;
                for(edgeIter iter = _edges.begin(); iter != _edges.end(); ++iter)
                {
                    EdgeItem *edge = *iter;
                    if(edge == item)
                    {
                        removeEdge(edge);
                    }
                }
                for(nodeIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
                {
                    NodeItem *node = *iter;
                    if(node == item)
                    {
                        removeNode(node);
                    }
                }
            }
        }
    }
        break;
    default:
        QGraphicsScene::keyPressEvent(event);
        break;
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
                _selecting = false;
                _fromNode = node;
                _mousePos = event->scenePos();
                update();
                return;
            }
        }
    }
    else if(event->button() == Qt::LeftButton)
    {
        // Rubber-band selection, but not when another item should handle this
        if(itemAt(event->scenePos()) || event->isAccepted())
        {
            QGraphicsScene::mousePressEvent(event);
            return;
        }

        _mousePos = event->scenePos();
        _mouseInitialPos = event->scenePos();
        _drawingEdge = false;
        _selecting = true;
        _fromNode = 0;
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
        if(event->button() != Qt::RightButton)
            return;

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

    if(_selecting)
    {
        if(event->button() != Qt::LeftButton)
            return;

        _selecting = false;
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
