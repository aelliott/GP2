/*!
 * \file
 */
#include "graphwidget.hpp"

#include "graphscene.hpp"

namespace Developer {

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    _scene = new GraphScene(this);

    QRect rect = geometry();
    QRectF sceneRect(rect);
    sceneRect.setWidth(sceneRect.width()-8);
    sceneRect.setHeight(sceneRect.height()-8);
    _scene->setSceneRect(sceneRect);

    setScene(_scene);
    setRenderHint(QPainter::Antialiasing, true);
}

Graph *GraphWidget::graph() const
{
    return _scene->graph();
}

void GraphWidget::setGraph(Graph *newGraph)
{
    _scene->setGraph(newGraph);
}

}
