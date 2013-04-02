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
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QDebug>

namespace Developer {

NodeItem::NodeItem(Node *node, QGraphicsItem *parent)
    : GraphItem(node->id(), node->label(), "node", parent)
    , _node(node)
    , _shape(Ellipse)
    , _isRoot(node->isRoot())
    , _hover(false)
{
    setZValue(NODE_Z_VALUE);

    setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    connect(this, SIGNAL(xChanged()), this, SLOT(positionChanged()));
    connect(this, SIGNAL(yChanged()), this, SLOT(positionChanged()));
}

NodeItem::NodeItem(const QString &nodeId, const QString &nodeLabel, bool root,
                   QGraphicsItem *parent)
    : GraphItem(nodeId, nodeLabel, "node", parent)
    , _node(0)
    , _shape(Ellipse)
    , _isRoot(root)
    , _hover(false)
{
    setZValue(NODE_Z_VALUE);

    setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    connect(this, SIGNAL(xChanged()), this, SLOT(positionChanged()));
    connect(this, SIGNAL(yChanged()), this, SLOT(positionChanged()));
}

bool NodeItem::isRoot() const
{
    return _isRoot;
}

Node *NodeItem::node() const
{
    return _node;
}

void NodeItem::setId(const QString &itemId)
{
    GraphItem::setId(itemId);

    if(_node != 0)
        _node->setId(itemId);
}

void NodeItem::setLabel(const QString &itemLabel)
{
    GraphItem::setLabel(itemLabel);

    if(_node != 0)
        _node->setLabel(itemLabel);
}

void NodeItem::setIsRoot(bool root)
{
    _isRoot = root;
}

QPainterPath NodeItem::shape() const
{
    QSettings settings;
    QFont font = settings.value("GraphView/Nodes/Font", qApp->font()
                                ).value<QFont>();

    QFontMetrics metrics(font);

    QRectF rect = boundingRect();
    // Leave space for the ID underneath
    rect.setHeight(rect.height()-(1+metrics.height()+1));
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
    QSettings settings;
    QFont font = settings.value("GraphView/Nodes/Font", qApp->font()
                                ).value<QFont>();

    QFontMetrics metrics(font);

    QRectF rect = boundingRect();
    // Leave space for the ID underneath
    rect.setHeight(rect.height()-(1+metrics.height()+1));
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
        borderWidth *= 1.5;

    qreal width  = borderWidth + leftPadding + metrics.width(label())
            + rightPadding + borderWidth;
    qreal height = borderWidth + topPadding + metrics.height() + bottomPadding
            + borderWidth + 1 + metrics.height() + 1;

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
                                       QColor(0x33,0x33,0x33)
                                         ).value<QColor>();
    qreal borderWidth = settings.value("GraphView/Nodes/Borders/Width", 2
                                       ).toDouble();
    QColor borderColour;
    QColor backgroundColour;
    if(option->state & QStyle::State_Selected)
    {
        borderColour = settings.value("GraphView/Nodes/Borders/SelectedColour",
                                      QColor(0xff,0xff,0x66) // yellow
                                      ).value<QColor>();
        backgroundColour = settings.value("GraphView/Nodes/SelectedBackground",
                                          QColor(0xff,0xff,0xcc) // light yellow
                                          ).value<QColor>();
    }
    else
    {
        if(_hover)
        {
            borderColour = settings.value("GraphView/Nodes/Borders/HoverColour",
                                          QColor(0xcc,0xcc,0xff) // blue
                                          ).value<QColor>();
            backgroundColour = settings.value("GraphView/Nodes/HoverBackground",
                                              QColor(0xdf,0xdf,0xff) // light blue
                                              ).value<QColor>();
        }
        else
        {
            borderColour = settings.value("GraphView/Nodes/Borders/Colour",
                                          QColor(0xaa,0xaa,0xff) // blue
                                          ).value<QColor>();
            backgroundColour = settings.value("GraphView/Nodes/Background",
                                              QColor(0xcc,0xcc,0xff) // light blue
                                              ).value<QColor>();
        }
    }

    QFontMetrics metrics(font);

    if(_isRoot)
        borderWidth *= 1.5;
    qreal textWidth  = metrics.width(label());
    qreal textHeight = metrics.height();

    painter->setBrush(backgroundColour);
    QPen pen(borderColour);
    pen.setWidth(borderWidth);
    painter->setPen(pen);

    QPainterPath path = shape();
    QRectF pathRect = path.boundingRect();
    painter->drawPath(path);

    painter->setPen(textColour);
    painter->setFont(font);
    painter->drawText(QRectF(leftPadding+borderWidth, topPadding+borderWidth,
                             textWidth, textHeight),
                      label());

    // Draw the node ID
    QColor idColour = textColour;
    idColour.setAlpha(80);
    painter->setPen(idColour);
    qreal xOffset = (pathRect.width()/2)-metrics.width(id())/2;
    qreal yOffset = pathRect.height() + metrics.height() + 1;
    painter->drawText(QPointF(xOffset, yOffset), id());
}

void NodeItem::positionChanged()
{
    _node->setPos(pos());
}

void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPainterPath path = shape();
    path.translate(scenePos());
    if(path.contains(event->scenePos()))
        _hover = true;
    else
        _hover = false;

    update();
}

void NodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPainterPath path = shape();
    path.translate(scenePos());
    if(path.contains(event->scenePos()))
        _hover = true;
    else
        _hover = false;

    update();
}

void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    _hover = false;

    update();
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();

    EditNodeDialog dialog(this);
    dialog.exec();
}

}
