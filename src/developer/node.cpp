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
#include "node.hpp"
#include "graph.hpp"

#include <QRegExp>

namespace Developer {

Node::Node(const QString &nodeId, const List &nodeLabel, const QPointF &nodePos, Graph *parent)
    : QObject(parent)
    , _id(nodeId)
    , _label(nodeLabel)
    , _pos(nodePos)
    , _isRoot(false)
    , _marked(false)
    , _parent(parent)
    , _phantom(false)
{
    if(_id.endsWith("(R)"))
    {
        _isRoot = true;
        _id.remove(QRegExp("\\((r|R)\\)$"));
    }
}

QString Node::id() const
{
    return _id;
}

List Node::label() const
{
    return _label;
}

QPointF Node::pos() const
{
    return _pos;
}

qreal Node::xPos() const
{
    return _pos.x();
}

qreal Node::yPos() const
{
    return _pos.y();
}

bool Node::isRoot() const
{
    return _isRoot;
}

bool Node::marked() const
{
    return _marked;
}

bool Node::isPhantomNode() const
{
    return _phantom;
}

std::vector<Edge *> Node::edges() const
{
    if(_parent == 0)
        return std::vector<Edge *>();

    return _parent->edges(id());
}

std::vector<Edge *> Node::edgesFrom() const
{
    if(_parent == 0)
        return std::vector<Edge *>();

    return _parent->edgesFrom(id());
}

std::vector<Edge *> Node::edgesTo() const
{
    if(_parent == 0)
        return std::vector<Edge *>();

    return _parent->edgesTo(id());
}

bool Node::hasEdgeOut() const
{
    if(_parent == 0)
        return false;

    return (_parent->edgeFrom(id()) != 0);
}

bool Node::hasEdgeIn() const
{
    if(_parent == 0)
        return false;

    return (_parent->edgeTo(id()) != 0);
}

Graph *Node::parent() const
{
    return _parent;
}

void Node::setId(const QString &nodeId)
{
    _id = nodeId;
    emit nodeChanged();
    emit idChanged(nodeId);
}

void Node::setLabel(const List &nodeLabel)
{
    _label = nodeLabel;
    emit nodeChanged();
    emit labelChanged(nodeLabel);
}

void Node::setPos(const QPointF &nodePos)
{
    _pos = nodePos;
}

void Node::setPos(qreal x, qreal y)
{
    setPos(QPointF(x, y));
}

void Node::setIsRoot(bool root)
{
    _isRoot = root;
    emit nodeChanged();
    emit isRootChanged(root);
}

void Node::setMarked(bool isMarked)
{
    _marked = isMarked;
    emit nodeChanged();
    emit markedChanged(isMarked);
}

void Node::setPhantom(bool phantom)
{
    _phantom = phantom;
    emit nodeChanged();
    emit isPhantomNodeChanged(phantom);
}

}
