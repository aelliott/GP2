/*!
 * \file
 */
#include "nodeitem.hpp"
#include "node.hpp"
#include "editnodedialog.hpp"

#include "graph.hpp"

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
    : GraphItem(node->id(), node->label().toString(), "node", parent)
    , _node(node)
    , _nodeShape(Ellipse)
    , _isRoot(node->isRoot())
    , _hover(false)
    , _marked(node->marked())
{
    setZValue(NODE_Z_VALUE);

    setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    connect(this, SIGNAL(xChanged()), this, SLOT(positionChanged()));
    connect(this, SIGNAL(yChanged()), this, SLOT(positionChanged()));

    if(node->isPhantomNode())
        setItemState(GraphItem_Deleted);
}

NodeItem::NodeItem(const QString &nodeId, const QString &nodeLabel, bool root,
                   QGraphicsItem *parent)
    : GraphItem(nodeId, nodeLabel, "node", parent)
    , _node(0)
    , _nodeShape(Ellipse)
    , _isRoot(root)
    , _hover(false)
{
    setZValue(NODE_Z_VALUE);

    setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    connect(this, SIGNAL(xChanged()), this, SLOT(positionChanged()));
    connect(this, SIGNAL(yChanged()), this, SLOT(positionChanged()));
}

void NodeItem::recalculate()
{
    _shape = QPainterPath();
    _shape = shape();
    _boundingRect = QRectF();
    _boundingRect = boundingRect();
    emit shapeChanged();
}

void NodeItem::addedEdge()
{
    emit edgeAdded();
}

bool NodeItem::isRoot() const
{
    return _isRoot;
}

bool NodeItem::marked() const
{
    return _marked;
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
    recalculate();

    if(_node != 0)
        _node->setLabel(List(itemLabel));
}

void NodeItem::setIsRoot(bool root)
{
    _isRoot = root;
    recalculate();

    if(_node != 0)
        _node->setIsRoot(root);
}

void NodeItem::setMarked(bool isMarked)
{
    _marked = isMarked;

    if(_node != 0)
        _node->setMarked(isMarked);
}

void NodeItem::deleteNode()
{
    setItemState(GraphItem::GraphItem_Deleted);

    if(_node != 0)
        _node->setPhantom(true);
}

void NodeItem::preserveNode()
{
    setItemState(GraphItem::GraphItem_Normal);

    if(_node != 0)
        _node->setPhantom(false);
}

QPainterPath NodeItem::shape() const
{
    if(!_shape.isEmpty())
        return _shape;

    QSettings settings;
    QFont font = settings.value("GraphView/Nodes/Font", qApp->font()
                                ).value<QFont>();

    QFontMetrics metrics(font);

    QRectF rect = boundingRect();
    // Leave space for the ID underneath
    rect.setHeight(rect.height()-(1+metrics.height()+1));
    QPainterPath path;
    switch(_nodeShape)
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
    if(!_boundingRect.isEmpty())
        return _boundingRect;

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

    // Aha, switch to British English and now this symbols is not already
    // assigned, man, the UK is just better isn't it?
    QColor borderColour = borderColor(option);
    QColor bgColour = backgroundColor(option);

    QFontMetrics metrics(font);

    if(_isRoot)
        borderWidth *= 1.5;
    qreal textWidth  = metrics.width(label());
    qreal textHeight = metrics.height();

    painter->setBrush(bgColour);
    QPen pen(borderColour);
    pen.setWidth(borderWidth);
    painter->setPen(pen);

    QPainterPath path = shape();
    QRectF pathRect = path.boundingRect();
    painter->drawPath(path);

    if(itemState() == GraphItem_Invalid)
    {
        QRectF destRect(pathRect.width()-16, pathRect.height()-16, 16, 16);
        QRectF sourceRect(0, 0, 16, 16);
        painter->drawPixmap(
                    destRect,
                    QPixmap(":/icons/small_exclamation.png"),
                    sourceRect
                    );
        setToolTip(tr("Node shares an ID with an edge in the LHS graph, this "
                      "is not permitted."));
    }

    painter->setPen(textColour);
    painter->setFont(font);
    painter->drawText(QRectF(leftPadding+borderWidth, topPadding+borderWidth,
                             textWidth, textHeight),
                      Qt::AlignCenter,
                      label());

    // Draw the node ID
    QColor idColour = textColour;
    idColour.setAlpha(80);
    painter->setPen(idColour);
    qreal xOffset = (pathRect.width()/2)-metrics.width(id())/2;
    qreal yOffset = pathRect.height() + metrics.height() + 1;
    painter->drawText(QPointF(xOffset, yOffset), id());
}

QColor NodeItem::backgroundColor(const QStyleOptionGraphicsItem *option) const
{
    QSettings settings;
    QColor ret;

    if(option->state & QStyle::State_Selected)
    {
        ret = settings.value("GraphView/Nodes/SelectedBackground",
                              QColor(0xff,0xff,0xcc) // light yellow
                              ).value<QColor>();
    }
    else
    {
        switch(itemState())
        {
        case GraphItem_New:
            ret = settings.value("GraphView/Nodes/Borders/ColourNew",
                                 QColor(0xcd,0xff,0xc6,0xaa) // green
                                 ).value<QColor>();
            break;
        case GraphItem_Deleted:
        case GraphItem_Invalid:
            ret = settings.value("GraphView/Nodes/Borders/ColourDeleted",
                                 QColor(0xff,0xaa,0xaa,0x55) // light red
                                 ).value<QColor>();
            break;
        case GraphItem_Normal:
        default:
            ret = settings.value("GraphView/Nodes/Background",
                                 QColor(0xcc,0xcc,0xff) // light blue
                                 ).value<QColor>();
            break;
        }

        // Is this a hover-ed node or a marked node? If yes, lighten or darken the
        // colour as appropriate
        if(_hover && !_marked)
            ret = ret.lighter(110);
        if(!_hover && _marked)
            ret = ret.darker(110);
    }


    return ret;
}

QColor NodeItem::borderColor(const QStyleOptionGraphicsItem *option) const
{
    QSettings settings;

    if(option->state & QStyle::State_Selected)
    {
        return settings.value("GraphView/Nodes/Borders/SelectedColour",
                              QColor(0xff,0xff,0x66) // yellow
                              ).value<QColor>();
    }
    else
    {
        if(_hover)
        {
            switch(itemState())
            {
            case GraphItem_New:
                return settings.value("GraphView/Nodes/Borders/HoverColourNew",
                                      QColor(0xcc,0xff,0xcc) // light green
                                      ).value<QColor>();
            case GraphItem_Deleted:
            case GraphItem_Invalid:
                return settings.value("GraphView/Nodes/Borders/HoverColourDeleted",
                                      QColor(0xff,0xaa,0xaa,0x55) // light red
                                      ).value<QColor>();
            case GraphItem_Normal:
            default:
                return settings.value("GraphView/Nodes/Borders/HoverColour",
                                      QColor(0xcc,0xcc,0xff) // blue
                                      ).value<QColor>();
            }

        }
        else
        {
            switch(itemState())
            {
            case GraphItem_New:
                return settings.value("GraphView/Nodes/Borders/HoverColourNew",
                                      QColor(0xaa,0xff,0xaa) // green
                                      ).value<QColor>();
            case GraphItem_Deleted:
            case GraphItem_Invalid:
                return settings.value("GraphView/Nodes/Borders/HoverColourDeleted",
                                      QColor(0xff,0x8f,0x8f,0x55) // red
                                      ).value<QColor>();
            case GraphItem_Normal:
            default:
                return settings.value("GraphView/Nodes/Borders/Colour",
                                      QColor(0xaa,0xaa,0xff) // blue
                                      ).value<QColor>();
            }

        }
    }

    // Shouldn't happen
    return QColor();
}

void NodeItem::positionChanged()
{
    if(_node != 0)
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
    event->accept();
    _hover = false;

    update();
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(node()->parent()->status() == GPFile::ReadOnly)
    {
        event->ignore();
        return;
    }

    event->accept();

    EditNodeDialog dialog(this);
    dialog.exec();
}

}
