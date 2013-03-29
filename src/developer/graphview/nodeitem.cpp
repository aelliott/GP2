/*!
 * \file
 */
#include "nodeitem.hpp"
#include "node.hpp"
#include "editnodedialog.hpp"

#include <QApplication>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QSettings>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

namespace Developer {

NodeItem::NodeItem(Node *node, QGraphicsItem *parent)
    : GraphItem(node->id(), node->label(), parent)
    , _shape(Ellipse)
    , _isRoot(node->isRoot())
{
    setZValue(NODE_Z_VALUE);
}

NodeItem::NodeItem(const QString &nodeId, const QString &nodeLabel, bool root,
                   QGraphicsItem *parent)
    : GraphItem(nodeId,nodeLabel,parent)
    , _shape(Ellipse)
    , _isRoot(root)
{
    setZValue(NODE_Z_VALUE);
}

bool NodeItem::isRoot() const
{
    return _isRoot;
}

void NodeItem::setIsRoot(bool root)
{
    _isRoot = root;
}

QPainterPath NodeItem::shape() const
{
    QSettings settings;
    QRectF rect = boundingRect();
    QPainterPath path;
    switch(_shape)
    {

    case Circle:
    {
        qreal size = qMin(rect.width(), rect.height());
        path.addEllipse(QRectF(0,0,size,size));
    }
        break;

    case Ellipse:
        path.addEllipse(rect);
        break;

    case Rectangle:
        path.addRect(rect);
        break;

    case RoundedRectangle:
    {
        qreal radius = settings.value("GraphView/Nodes/CornerRadius", 6
                                      ).toDouble();
        path.addRoundedRect(rect, radius, radius);
    }
        break;
    }

    return path;
}

QList<QPointF> NodeItem::intersection(QLineF line) const
{
    QList<QPointF> intersectionPoints;
    QPainterPath path = shape();
    path.translate(scenePos());
    QPolygonF polygon = path.toFillPolygon();
    if(polygon.size() < 3)
    {
        qDebug() << QString("Polygon is not two dimensional, only contains %1 "
                            "points.").arg(polygon.size());
        return intersectionPoints;
    }

    for(int i = 0; i < polygon.size(); ++i)
    {
        QPointF p1 = polygon.at(i);
        QPointF p2;
        if(i == (polygon.size()-1))
            p2 = polygon.at(0);
        else
            p2 = polygon.at(i+1);

        QLineF shapeEdge = QLineF(p1, p2);

        QPointF intersectPoint;
        if(line.intersect(shapeEdge, &intersectPoint)
                == QLineF::BoundedIntersection)
            intersectionPoints << intersectPoint;
    }

    return intersectionPoints;
}

QPointF NodeItem::centerPos() const
{
    QRectF rect = boundingRect();
    QPointF tmp = pos();

    return QPointF(tmp.x() + rect.width()/2,
                   tmp.y() + rect.height()/2);
}

QRectF NodeItem::boundingRect() const
{
    QSettings settings;
    qreal topPadding    = settings.value("GraphView/Nodes/Padding/Top", 6
                                         ).toDouble();
    qreal rightPadding  = settings.value("GraphView/Nodes/Padding/Right", 8
                                         ).toDouble();
    qreal bottomPadding = settings.value("GraphView/Nodes/Padding/Bottom", 6
                                         ).toDouble();
    qreal leftPadding   = settings.value("GraphView/Nodes/Padding/Left", 8
                                         ).toDouble();
    qreal borderWidth = settings.value("GraphView/Nodes/Borders/Width", 2
                                       ).toDouble();
    QFont font = settings.value("GraphView/Nodes/Font", qApp->font()
                                ).value<QFont>();

    QFontMetrics metrics(font);

    if(_isRoot)
        borderWidth *= 2;

    qreal width  = borderWidth + leftPadding + metrics.width(label())
            + rightPadding + borderWidth;
    qreal height = borderWidth + topPadding + metrics.height() + bottomPadding
            + borderWidth;

    return QRectF(0, 0, width, height);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(widget);
    QSettings settings;

    qreal topPadding    = settings.value("GraphView/Nodes/Padding/Top", 6
                                         ).toDouble();
    qreal leftPadding   = settings.value("GraphView/Nodes/Padding/Left", 8
                                         ).toDouble();
    QFont font = settings.value("GraphView/Nodes/Font", qApp->font()
                                ).value<QFont>();
    QColor textColour = settings.value("GraphView/Nodes/TextColour",
                                       QColor(0x44,0x44,0x44)
                                         ).value<QColor>();
    qreal borderWidth = settings.value("GraphView/Nodes/Borders/Width", 2
                                       ).toDouble();
    QColor borderColour = settings.value("GraphView/Nodes/Borders/Colour",
                                         QColor(0xff,0xff,0x66) // yellow
                                         ).value<QColor>();
    QColor backgroundColour = settings.value("GraphView/Nodes/Background",
                                             QColor(0xff,0xff,0xcc) // light yellow
                                             ).value<QColor>();

    QFontMetrics metrics(font);

    if(_isRoot)
        borderWidth *= 2;
    qreal textWidth  = metrics.width(label());
    qreal textHeight = metrics.height();

    painter->setBrush(backgroundColour);
    QPen pen(borderColour);
    pen.setWidth(borderWidth);
    painter->setPen(pen);

    painter->drawPath(shape());

    painter->setPen(textColour);
    painter->setFont(font);
    painter->drawText(QRectF(leftPadding+borderWidth, topPadding+borderWidth,
                             textWidth, textHeight),
                      label());
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();

    EditNodeDialog dialog;
    dialog.exec();
}

}
