#ifndef EDGE_HPP
#define EDGE_HPP

#include "node.hpp"
#include <QString>

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
    Edge(Node *fromNode, Node *toNode, const QString &edgeLabel = QString());

    Node *from() const;
    Node *to() const;

    QString label() const;

    void setFrom(Node *fromNode);
    void setTo(Node *toNode);

    void setLabel(const QString &edgeLabel);

private:
    Node *_from;
    Node *_to;
    QString _label;
};

}

#endif // EDGE_HPP
