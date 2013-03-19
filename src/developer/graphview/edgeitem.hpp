/*!
 * \file
 */
#ifndef EDGEITEM_HPP
#define EDGEITEM_HPP

#include "nodeitem.hpp"

namespace Developer {

/*!
 * \brief The EdgeItem class provides a graphical representation of a graph Edge
 *  within the graph visualisation
 */
class EdgeItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit EdgeItem(QGraphicsItem *parent = 0);
    
signals:
    
public slots:
    
};

}

#endif // EDGEITEM_HPP