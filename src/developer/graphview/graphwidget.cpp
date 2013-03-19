/*!
 * \file
 */
#include "graphwidget.hpp"

namespace Developer {

GraphWidget::GraphWidget(Graph *newGraph, QWidget *parent)
    : QWidget(parent)
    , _graph(newGraph)
    , _idCounter(0)
{
}

Graph *GraphWidget::graph() const
{
    return _graph;
}

void GraphWidget::setGraph(Graph *newGraph)
{
    //NOTE: I'm going to assume that the Graph object which this widget holds
    //      a pointer to is not controlled by the widget, therefore it is not
    //      the responsibility of this widget to clean up its memory
    //if(_graph != 0)
    //    delete _graph;
    _graph = newGraph;
}

}
