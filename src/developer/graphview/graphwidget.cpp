/*!
 * \file
 */
#include "graphwidget.hpp"

#include "graphscene.hpp"

#include <QKeyEvent>
#include <QWheelEvent>
#include <QFocusEvent>
#include <cmath>

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
    setViewportUpdateMode(BoundingRectViewportUpdate);
    //setDragMode(QGraphicsView::RubberBandDrag);
}

Graph *GraphWidget::graph() const
{
    return _scene->graph();
}

void GraphWidget::setGraph(Graph *newGraph)
{
    _scene->setGraph(newGraph);
}

void GraphWidget::layoutTree(LayoutDirections direction)
{
    _scene->layoutTree(direction);
}

void GraphWidget::layoutSugiyama()
{
    _scene->layoutSugiyama();
}

void GraphWidget::layoutRadialTree()
{
    _scene->layoutRadialTree();
}

void GraphWidget::layoutFPP()
{
    _scene->layoutFPP();
}

void GraphWidget::layoutPlanarDraw()
{
    _scene->layoutPlanarDraw();
}

void GraphWidget::layoutPlanarStraight()
{
    _scene->layoutPlanarStraight();
}

void GraphWidget::layoutSchnyder()
{
    _scene->layoutSchnyder();
}

void GraphWidget::layoutPlanarizationGrid()
{
    _scene->layoutPlanarizationGrid();
}

void GraphWidget::layoutCircular()
{
    _scene->layoutCircular();
}

void GraphWidget::layoutSpring()
{
    _scene->layoutSpring();
}

void GraphWidget::layoutDavidsonHarel()
{
    _scene->layoutDavidsonHarel();
}

void GraphWidget::layoutFMMM()
{
    _scene->layoutFMMM();
}

void GraphWidget::layoutGEM()
{
    _scene->layoutGEM();
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_0:
        if(event->modifiers() & Qt::ControlModifier)
            scale(1, 1);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2.0, event->delta() / 240.0));
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
    scene()->setSceneRect(scene()->itemsBoundingRect());
}

void GraphWidget::focusInEvent(QFocusEvent *event)
{
    QGraphicsView::focusInEvent(event);

    emit graphHasFocus(this);
}

void GraphWidget::focusOutEvent(QFocusEvent *event)
{
    QGraphicsView::focusOutEvent(event);

    emit graphLostFocus(this);
}

}
