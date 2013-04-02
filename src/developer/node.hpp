/*!
 * \file
 */
#ifndef NODE_HPP
#define NODE_HPP

#include <QString>
#include <QPointF>

namespace Developer {

/*!
 * \brief The Node class represents a node within a GP graph
 */
class Node
{
public:
    Node(const QString &nodeId, const QString &nodeLabel = QString(),
         const QPointF &nodePos = QPointF());

    QString id() const;
    QString label() const;
    QPointF pos() const;
    qreal xPos() const;
    qreal yPos() const;
    bool isRoot() const;

    void setId(const QString &nodeId);
    void setLabel(const QString &nodeLabel);
    void setPos(const QPointF &nodePos);
    void setPos(qreal x, qreal y);
    void setIsRoot(bool root);

private:
    QString _id;
    QString _label;
    QPointF _pos;
    bool _isRoot;
};

}

#endif // NODE_HPP
