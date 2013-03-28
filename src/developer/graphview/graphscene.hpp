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

    QMap<int, EdgeItem*> _edges;
    QMap<int, NodeItem*> _nodes;

    typedef QMap<int, EdgeItem*>::iterator edgeIter;
    typedef QMap<int, NodeItem*>::iterator nodeIter;
    typedef QMap<int, EdgeItem*>::const_iterator edgeConstIter;
    typedef QMap<int, NodeItem*>::const_iterator nodeConstIter;
};

}

#endif // GRAPHSCENE_HPP
