/*!
 * \file
 */
#ifndef GRAPHSCENE_HPP
#define GRAPHSCENE_HPP

#include <QGraphicsScene>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

// Implicitly brings in nodeitem.hpp
#include "edgeitem.hpp"
#include "../graph.hpp"

namespace Developer {

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphScene(QObject *parent = 0);

    Graph *graph() const;
    void setGraph(Graph *newGraph);

    EdgeItem *edgeItem(const QString &id) const;

    void addNodeItem(NodeItem *nodeItem, const QPointF &position);
    void addEdgeItem(EdgeItem *edgeItem);

    void layoutSugiyama();
    void layoutCircular();
    void layoutSpring();

    void resizeToContents();

public slots:
    void addNode(const QPointF &position);
    void addNode(qreal x, qreal y);

    void removeEdge(EdgeItem *edge);
    void removeNode(NodeItem *node);

    void nodeIdChanged(QString oldId, QString newId);

signals:
    void nodeAdded(NodeItem *nodeItem);
    void edgeAdded(EdgeItem *edgeItem);

protected:
    void layoutInit();
    void layoutApply();

    void drawForeground(QPainter *painter, const QRectF &rect);

    void keyPressEvent(QKeyEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    Graph *_graph;
    bool _internalGraph;

    bool _drawingEdge;
    NodeItem *_fromNode;
    QPointF _mousePos;

    QMap<QString, EdgeItem*> _edges;
    QMap<QString, NodeItem*> _nodes;

    ogdf::Graph _g;
    ogdf::GraphAttributes _ga;
    QMap<QString, ogdf::node> _nodeMap;

    typedef QMap<QString, EdgeItem*>::iterator edgeIter;
    typedef QMap<QString, NodeItem*>::iterator nodeIter;
    typedef QMap<QString, EdgeItem*>::const_iterator edgeConstIter;
    typedef QMap<QString, NodeItem*>::const_iterator nodeConstIter;
};

}

#endif // GRAPHSCENE_HPP
