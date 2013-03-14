#ifndef GRAPHWIDGET_HPP
#define GRAPHWIDGET_HPP

#include <QWidget>

// Implicitly brings in nodeitem.hpp
#include "edgeitem.hpp"
#include "../graph.hpp"

namespace Developer {

/*!
 * \brief Provides a top-level widget for representing a graph visualisation
 *
 * This widget provides a wrapper around the graph visualisation code in GP
 * Developer. It contains a graphics view with NodeItem and EdgeItem objects
 * tied to the Graph passed to the widget.
 */
class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(Graph *newGraph = 0, QWidget *parent = 0);

    Graph *graph() const;
    void setGraph(Graph *newGraph);

private:
    Graph *_graph;
    int _idCounter;

    QMap<int, EdgeItem*> _edges;
    QMap<int, NodeItem*> _nodes;

    typedef QMap<int, EdgeItem*>::const_iterator edgeiter;
    typedef QMap<int, NodeItem*>::const_iterator nodeiter;
};

}

#endif // GRAPHWIDGET_HPP
