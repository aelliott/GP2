/*!
 * \file
 */
#ifndef GRAPHITEM_HPP
#define GRAPHITEM_HPP

#include <QGraphicsObject>

namespace Developer {

class GraphItem : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit GraphItem(const QString &itemId,
                       const QString &itemLabel = QString(),
                       QGraphicsItem *parent = 0);

    QString id() const;
    QString label() const;

    void setId(const QString &itemId);
    void setLabel(const QString &itemLabel);

protected:
    QString _id;
    QString _label;
};

}

#endif // GRAPHITEM_HPP
