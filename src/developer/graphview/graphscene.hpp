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

public slots:
    void addNode(const QPointF &position);
    void addNode(qreal x, qreal y);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    Graph *_graph;
    bool _internalGraph;
    NodeItem *_previousNode;

    QMap<QString, EdgeItem*> _edges;
    QMap<QString, NodeItem*> _nodes;

    typedef QMap<QString, EdgeItem*>::iterator edgeIter;
    typedef QMap<QString, NodeItem*>::iterator nodeIter;
    typedef QMap<QString, EdgeItem*>::const_iterator edgeConstIter;
    typedef QMap<QString, NodeItem*>::const_iterator nodeConstIter;
};

}

#endif // GRAPHSCENE_HPP
