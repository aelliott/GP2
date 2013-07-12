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
#include "graphitem.hpp"

namespace Developer {

GraphItem::GraphItem(const QString &itemId, const QString &itemLabel, const QString &itemType,
                     QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , _id(itemId)
    , _label(itemLabel)
    , _itemType(itemType)
    , _itemState(GraphItem_Normal)
{
}

QString GraphItem::id() const
{
    return _id;
}

QString GraphItem::label() const
{
    return _label;
}

QString GraphItem::itemType() const
{
    return _itemType;
}

GraphItem::ItemState GraphItem::itemState() const
{
    return _itemState;
}

void GraphItem::setId(const QString &itemId)
{
    if(itemId != _id)
    {
        emit idChanged(_id, itemId);
        _id = itemId;
    }
}

void GraphItem::setLabel(const QString &itemLabel)
{
    if(itemLabel != _label)
    {
        _label = itemLabel;
    }
}

void GraphItem::setItemState(ItemState state)
{
    _itemState = state;
}

}
