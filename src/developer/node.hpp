/*!
 * \file
 */
#ifndef NODE_HPP
#define NODE_HPP

#include <QPointF>

#include "list.hpp"

namespace Developer {

class Graph;
class Edge;

/*!
 * \brief The Node class represents a node within a GP graph object
 */
class Node
{
public:
    Node(const QString &nodeId, const List &nodeLabel = List(),
         const QPointF &nodePos = QPointF(), Graph *parent = 0);
    Node(const QString &nodeId, Graph *parent);

    QString id() const;
    List label() const;
    QPointF pos() const;
    qreal xPos() const;
    qreal yPos() const;
    bool isRoot() const;
    bool marked() const;
    bool isPhantomNode() const;

    std::vector<Edge *> edges() const;
    std::vector<Edge *> edgesFrom() const;
    std::vector<Edge *> edgesTo() const;

    bool hasEdgeIn() const;
    bool hasEdgeOut() const;

    Graph *parent() const;

    void setId(const QString &nodeId);
    void setLabel(const List &nodeLabel);
    void setPos(const QPointF &nodePos);
    void setPos(qreal x, qreal y);
    void setIsRoot(bool root);
    void setMarked(bool isMarked);
    void setPhantom(bool phantom);

private:
    QString _id;
    List _label;
    QPointF _pos;
    bool _isRoot;
    bool _marked;
    Graph *_parent;
    bool _phantom;
};

}

#endif // NODE_HPP
