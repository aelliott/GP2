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
#ifndef GRAPHITEM_HPP
#define GRAPHITEM_HPP

#include <QGraphicsObject>

#include "global.hpp"

namespace Developer {

class GraphItem : public QGraphicsObject
{
    Q_OBJECT

public:
    enum ItemState
    {
        GraphItem_Normal,
        GraphItem_New,
        GraphItem_Deleted,
        GraphItem_Invalid
    };

    explicit GraphItem(const QString &itemId,
                       const QString &itemLabel = QString(),
                       const QString &itemType = QString(),
                       QGraphicsItem *parent = 0);

    QString id() const;
    QString label() const;
    QString itemType() const;
    ItemState itemState() const;

    void setId(const QString &itemId);
    void setLabel(const QString &itemLabel);
    void setItemState(ItemState state);

signals:
    void idChanged(QString oldId, QString newId);

protected:
    QString _id;
    QString _label;
    QString _itemType;
    ItemState _itemState;
};

}

#endif // GRAPHITEM_HPP
