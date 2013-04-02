/*!
 * \file
 */
#ifndef GRAPHWIDGET_HPP
#define GRAPHWIDGET_HPP

#include <QGraphicsView>

namespace Developer {

class Graph;
class GraphScene;

/*!
 * \brief Provides a top-level widget for representing a graph visualisation
 *
 * This widget provides a wrapper around the graph visualisation code in GP
 * Developer. It contains a graphics view with NodeItem and EdgeItem objects
 * tied to the Graph passed to the widget.
 */
class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);

    Graph *graph() const;
    void setGraph(Graph *newGraph);

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);

private:
    GraphScene *_scene;
};

}

#endif // GRAPHWIDGET_HPP
