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
class Edge : public QObject
{
    Q_OBJECT

public:
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

signals:
    void edgeChanged();
    void idChanged(QString id);
    void fromChanged(Node *fromNode);
    void toChanged(Node *toNode);
    void labelChanged(List label);
    void isPhantomEdgeChanged(bool phantom);

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
