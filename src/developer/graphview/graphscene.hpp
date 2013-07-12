/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GRAPHSCENE_HPP
#define GRAPHSCENE_HPP

#include <QGraphicsScene>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

// Implicitly brings in nodeitem.hpp
#include "graphview/edgeitem.hpp"
#include "graph.hpp"
#include "global.hpp"

namespace Developer {

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphScene(QObject *parent = 0);

    Graph *graph() const;
    void setGraph(Graph *newGraph);
    Graph *linkedGraph() const;
    void setLinkedGraph(Graph *linkGraph);
    bool readOnly() const;
    void setReadOnly(bool readOnlyFlag);

    NodeItem *node(const QString &id) const;
    EdgeItem *edge(const QString &id) const;

    void addNodeItem(NodeItem *nodeItem, const QPointF &position);
    void addEdgeItem(EdgeItem *edgeItem);

    void layoutTree(LayoutDirections direction = DEFAULT_LAYOUT_DIRECTION);
    void layoutSugiyama();
    void layoutRadialTree();
    void layoutFPP();
    void layoutPlanarDraw();
    void layoutPlanarStraight();
    void layoutSchnyder();
    void layoutPlanarizationGrid();
    void layoutCircular();
    void layoutSpring();
    void layoutDavidsonHarel();
    void layoutFMMM();
    void layoutGEM();

    void resizeToContents();

public slots:
    void addNode(const QPointF &position, bool automatic = false);
    void addNode(qreal x, qreal y);

    void removeEdge(EdgeItem *edge);
    void removeNode(NodeItem *node);

    void nodeIdChanged(QString oldId, QString newId);

signals:
    void nodeAdded(NodeItem *nodeItem);
    void edgeAdded(EdgeItem *edgeItem);

protected slots:
    void linkedGraphAddedNode(Node *nodeItem);
    void linkedGraphAddedEdge(Edge *edgeItem);

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
    Graph *_linkedGraph;
    bool _readOnly;
    bool _internalGraph;

    bool _drawingEdge;
    bool _selecting;
    NodeItem *_fromNode;
    QPointF _mouseInitialPos;
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
