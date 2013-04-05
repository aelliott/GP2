/*!
 * \file
 */
#ifndef GRAPHSCENE_HPP
#define GRAPHSCENE_HPP

#include <QGraphicsScene>

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

    void addNodeItem(NodeItem *nodeItem, const QPointF &position);
    void addEdgeItem(EdgeItem *edgeItem);

    void layoutSugiyama();
    void layoutCircular();

public slots:
    void addNode(const QPointF &position);
    void addNode(qreal x, qreal y);

    void nodeIdChanged(QString oldId, QString newId);

signals:
    void nodeAdded(NodeItem *nodeItem);
    void edgeAdded(EdgeItem *edgeItem);

protected:
    void drawForeground(QPainter *painter, const QRectF &rect);

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

    typedef QMap<QString, EdgeItem*>::iterator edgeIter;
    typedef QMap<QString, NodeItem*>::iterator nodeIter;
    typedef QMap<QString, EdgeItem*>::const_iterator edgeConstIter;
    typedef QMap<QString, NodeItem*>::const_iterator nodeConstIter;
};

}

#endif // GRAPHSCENE_HPP
