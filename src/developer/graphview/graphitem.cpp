/*!
 * \file
 */
#include "graphitem.hpp"

namespace Developer {

GraphItem::GraphItem(const QString &itemId, const QString &itemLabel,
                     QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , _id(itemId)
    , _label(itemLabel)
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

void GraphItem::setId(const QString &itemId)
{
    _id = itemId;
}

void GraphItem::setLabel(const QString &itemLabel)
{
    _label = itemLabel;
}

}
