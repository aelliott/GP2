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
#ifndef NODE_HPP
#define NODE_HPP

#include <QPointF>
#include <QObject>

#include "list.hpp"

namespace Developer {

class Graph;
class Edge;

/*!
 * \brief The Node class represents a node within a GP graph object
 */
class Node : public QObject
{
    Q_OBJECT

public:
    Node(const QString &nodeId, const List &nodeLabel = List(),
         const QPointF &nodePos = QPointF(), Graph *parent = 0);

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

signals:
    void nodeChanged();
    void idChanged(QString id);
    void labelChanged(List label);
    void isRootChanged(bool root);
    void markedChanged(bool isMarked);
    void isPhantomNodeChanged(bool phantom);

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
