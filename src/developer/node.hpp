/*!
 * \file
 */
#ifndef NODE_HPP
#define NODE_HPP

#include <QString>
#include <QPointF>
#include <vector>

namespace Developer {

class Graph;
class Edge;

/*!
 * \brief The Node class represents a node within a GP graph
 */
class Node
{
public:
    Node(const QString &nodeId, const QString &nodeLabel = QString(),
         const QPointF &nodePos = QPointF(), Graph *parent = 0);
    Node(const QString &nodeId, Graph *parent);

    QString id() const;
    QString label() const;
    QPointF pos() const;
    qreal xPos() const;
    qreal yPos() const;
    bool isRoot() const;

    std::vector<Edge *> edges() const;
    std::vector<Edge *> edgesFrom() const;
    std::vector<Edge *> edgesTo() const;

    bool hasEdgeIn() const;
    bool hasEdgeOut() const;

    Graph *parent() const;

    void setId(const QString &nodeId);
    void setLabel(const QString &nodeLabel);
    void setPos(const QPointF &nodePos);
    void setPos(qreal x, qreal y);
    void setIsRoot(bool root);

private:
    QString _id;
    QString _label;
    QPointF _pos;
    bool _isRoot;
    Graph *_parent;
};

}

#endif // NODE_HPP
