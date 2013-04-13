/*!
 * \file
 */
#ifndef EDGE_HPP
#define EDGE_HPP

#include "node.hpp"

namespace Developer {

/*!
 * \brief The Edge class represents a directed edge in a graph
 *
 * This class could be extended to consider undirected edges, but GP currently
 * does not use them.
 */
class Edge
{
public:
    Edge(const QString &edgeId, Node *fromNode, Node *toNode, Graph *parent);
    Edge(const QString &edgeId, Node *fromNode, Node *toNode,
         const List &edgeLabel, Graph *parent);

    QString id() const;
    Node *from() const;
    Node *to() const;
    List label() const;
    bool isPhantomEdge() const;

    Graph *parent() const;

    void setId(const QString &edgeId);
    void setFrom(Node *fromNode);
    void setTo(Node *toNode);
    void setLabel(const List &edgeLabel);
    void setPhantom(bool phantom);

private:
    Graph *_parent;

    QString _id;
    Node *_from;
    Node *_to;
    List _label;
    bool _phantom;
};

}

#endif // EDGE_HPP
