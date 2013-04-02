/*!
 * \file
 */
#include "edgeitem.hpp"
#include "edge.hpp"
#include "editedgedialog.hpp"

#include <QApplication>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QSettings>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsScene>

namespace Developer {

EdgeItem::EdgeItem(Edge *edge, NodeItem *edgeFrom, NodeItem *edgeTo,
                   QGraphicsItem *parent)
    : GraphItem(edge->id(), edge->label(), "edge", parent)
    , _hover(false)
    , _fromAnchor(false)
    , _toAnchor(false)
{
    setZValue(EDGE_Z_VALUE);

    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable);

    setFrom(edgeFrom);
    setTo(edgeTo);
}

EdgeItem::EdgeItem(const QString &edgeId, NodeItem *edgeFrom, NodeItem *edgeTo,
                   const QString &edgeLabel, QGraphicsItem *parent)
    : GraphItem(edgeId, edgeLabel, "edge", parent)
    , _from(edgeFrom)
    , _to(edgeTo)
    , _hover(false)
{
    setZValue(EDGE_Z_VALUE);

    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable);

    setFrom(edgeFrom);
    setTo(edgeTo);
}

NodeItem *EdgeItem::from() const
{
    return _from;
}

NodeItem *EdgeItem::to() const
{
    return _to;
}

void EdgeItem::setFrom(NodeItem *edgeFrom)
{
    _from = edgeFrom;
    connect(_from, SIGNAL(xChanged()), this, SLOT(nodeMoved()));
    connect(_from, SIGNAL(yChanged()), this, SLOT(nodeMoved()));
}

void EdgeItem::setTo(NodeItem *edgeTo)
{
    _to = edgeTo;
    connect(_to, SIGNAL(xChanged()), this, SLOT(nodeMoved()));
    connect(_to, SIGNAL(yChanged()), this, SLOT(nodeMoved()));
}

QLineF EdgeItem::line() const
{
    QLineF initial = QLineF(_from->centerPos(), _to->centerPos());
    QList<QPointF> fromIntersection = _from->intersection(initial);
    QList<QPointF> toIntersection = _to->intersection(initial);

    if(fromIntersection.count() < 1 || toIntersection.count() < 1)
        return initial;

    return QLineF(fromIntersection.at(0), toIntersection.at(0));
}

QPolygonF EdgeItem::polygon() const
{
    QSettings settings;
    QFont font = settings.value("GraphView/Edges/Font", qApp->font()
                                ).value<QFont>();
    QFontMetrics metrics(font);

    QLineF edgeLine = line();
    QLineF width = edgeLine;
    qreal angle = width.angle();
    angle += 90; if(angle > 360) angle -= 360;
    width.setAngle(angle);
    width.setLength(metrics.height() + 2.0);

    // Compute the points required, p1 is the edgeLine.p1() value /plus/ the
    // height of a line of text, this is currently width.p2()
    QVector<QPointF> points;
    points << width.p2();
    // And turn it around to get the point below the line
    angle -= 180; if(angle < 0) angle += 360;
    width.setAngle(angle);
    points << width.p2();

    // Now move width to edgeLine.p2()
    width.translate(edgeLine.p2() - edgeLine.p1());

    // Now width.p2() is edgeLine.p2() /minus/ the height of a line of text
    points << width.p2();
    // And turn it back around to get the final point above the line
    angle += 180; if(angle > 360) angle -= 360;
    width.setAngle(angle);
    points << width.p2();

    points << points.at(0);

    return QPolygonF(points);
}

QPolygonF EdgeItem::edgePolygon(double padding) const
{
    // This method works exactly like the above one, except that the width of
    // the produced polygon is just sufficient to contain the arrow itself
    QSettings settings;
    qreal arrowSize = settings.value("GraphView/Edges/ArrowSize", 9).toDouble();

    QLineF edgeLine = line();
    QLineF width = edgeLine;
    qreal angle = width.angle();
    angle += 90; if(angle > 360) angle -= 360;
    width.setAngle(angle);
    width.setLength((arrowSize/2) + padding);

    QVector<QPointF> points;
    points << width.p2();
    angle -= 180; if(angle < 0) angle += 360;
    width.setAngle(angle);
    points << width.p2();

    width.translate(edgeLine.p2() - edgeLine.p1());

    points << width.p2();
    angle += 180; if(angle > 360) angle -= 360;
    width.setAngle(angle);
    points << width.p2();

    points << points.at(0);

    return QPolygonF(points);
}

QRectF EdgeItem::boundingRect() const
{
    return polygon().boundingRect();
}

QPainterPath EdgeItem::path() const
{
    QSettings settings;
    qreal arrowSize = settings.value("GraphView/Edges/ArrowSize", 9).toDouble();
    qreal lineWidth = settings.value("GraphView/Edges/LineWidth", 1.5).toDouble();

    QLineF edgeLine = line();
    QLineF drawLine = edgeLine;
    // Compensate for pen width
    drawLine.setLength(drawLine.length()-(lineWidth+0.5));
    QPainterPath painterPath(drawLine.p1());
    painterPath.lineTo(drawLine.p2());

    // Abuse QLineF's ability to transform lines in order to draw a triangle
    // at the end of this edge as follows:

    // Get the current angle
    qreal angle = drawLine.angle();
    // Shift it around by 150 degrees, this leaves 30 degrees as the internal
    // angle which is what we want as the edge cuts the equilateral triangle's
    // 60 degree internal angles in half
    // If we overshoot and get negative simply add 360 to fix it
    angle -= 150; if(angle < 0) angle += 360;
    // Move the line such that p1 is now where p2 sat
    drawLine.translate(drawLine.p2()-drawLine.p1());
    // Resize the line to be the length of an edge of the triangle
    drawLine.setLength(arrowSize);
    // Set the new angle, p2 is now in the correct place
    drawLine.setAngle(angle);
    // Draw a line to this point
    painterPath.lineTo(drawLine.p2());

    // Now that we've done the initial line the rest are all relative to the
    // equilateral triangle, and therefore we are moving to produce 60 degree
    // internal angles
    angle -= 120; if(angle < 0) angle += 360;
    // The rest is exactly the same
    drawLine.translate(drawLine.p2()-drawLine.p1());
    drawLine.setLength(arrowSize);
    drawLine.setAngle(angle);
    painterPath.lineTo(drawLine.p2());

    // One last time to get us back to our origin
    angle -= 120; if(angle < 0) angle += 360;
    drawLine.translate(drawLine.p2()-drawLine.p1());
    drawLine.setLength(arrowSize);
    drawLine.setAngle(angle);
    painterPath.lineTo(drawLine.p2());

    return painterPath;
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(widget)

    QSettings settings;
    QFont font = settings.value("GraphView/Edges/Font", qApp->font()
                                ).value<QFont>();
    qreal lineWidth = settings.value("GraphView/Edges/LineWidth", 1.5).toDouble();
    QFontMetrics metrics(font);
    QColor lineColour;
    if(option->state & QStyle::State_Selected)
    {
        lineColour  = settings.value("GraphView/Edges/SelectedColour",
                                     QColor(0xcc,0xcc,0x33)).value<QColor>();
    }
    else
    {
        if(_hover)
        {
            lineColour  = settings.value("GraphView/Edges/HoverColour",
                                         QColor(0x33,0x33,0xdd)).value<QColor>();
        }
        else
        {
           lineColour  = settings.value("GraphView/Edges/LineColour",
                                        QColor(0x33,0x33,0x33)).value<QColor>();
        }
    }

    if(SHOW_VISUALISATION_DEBUG)
    {
        painter->setPen(DEBUG_COLOUR);
        painter->drawRect(boundingRect());
        painter->drawPolygon(polygon());
        painter->drawPolygon(edgePolygon());
    }

    QPen pen(lineColour);
    pen.setWidth(lineWidth);
    painter->setPen(pen);
    painter->setBrush(lineColour);
    painter->setFont(font);

    // Draw the arrow
    QLineF edgeLine = line();
    painter->drawPath(path());

    // Now draw the label
    QPointF midPoint((edgeLine.p1().x() + edgeLine.p2().x())/2,
                     (edgeLine.p1().y() + edgeLine.p2().y())/2);
    painter->translate(midPoint);
    qreal angle = edgeLine.angle();
    bool flipped = false;
    if(angle > 90 && angle < 270) { angle -= 180; flipped = true; }
    if(angle < 0) angle += 360;
    painter->rotate(-angle);
    qreal xOffset = metrics.width(label())/2;
    painter->drawText(QPointF(-xOffset,-3), label());

    // Draw the edge ID
    QColor idColour = lineColour;
    idColour.setAlpha(80);
    painter->setPen(idColour);
    xOffset = metrics.width(id())/2;
    qreal yOffset = metrics.height()-3;
    painter->drawText(QPointF(-xOffset, yOffset), id());

    QColor anchorColour(Qt::lightGray);
    painter->setPen(anchorColour);
    anchorColour.setAlpha(80);
    painter->setBrush(anchorColour);

    // Draw anchor markings
    if(flipped)
        xOffset = -(edgeLine.length()/2);
    else
        xOffset = edgeLine.length()/2;

    if(_fromAnchor)
        painter->drawRect(QRectF(-xOffset - 6, -6, 12, 12));

    if(_toAnchor)
        painter->drawRect(QRectF(xOffset - 6, -6, 12, 12));
}

void EdgeItem::nodeMoved()
{
    update();
}

void EdgeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QLineF edgeLine = line();
    if(edgePolygon().containsPoint(event->scenePos(), Qt::OddEvenFill))
    {
        event->accept();
        _hover = true;

        if((event->scenePos()-edgeLine.p1()).manhattanLength() < 6)
            _fromAnchor = true;
        else
            _fromAnchor = false;

        if((event->scenePos()-edgeLine.p2()).manhattanLength() < 6)
            _toAnchor = true;
        else
            _toAnchor = false;
    }
    else
    {
        event->ignore();
        _hover = false;
        _fromAnchor = false;
        _toAnchor = false;
    }

    update();
}

void EdgeItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QLineF edgeLine = line();
    if(edgePolygon().containsPoint(event->scenePos(), Qt::OddEvenFill))
    {
        event->accept();
        _hover = true;

        if((event->scenePos()-edgeLine.p1()).manhattanLength() < 6)
            _fromAnchor = true;
        else
            _fromAnchor = false;

        if((event->scenePos()-edgeLine.p2()).manhattanLength() < 6)
            _toAnchor = true;
        else
            _toAnchor = false;
    }
    else
    {
        event->ignore();
        _hover = false;
        _fromAnchor = false;
        _toAnchor = false;
    }

    update();
}

void EdgeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    _hover = false;

    update();
}

void EdgeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && _hover)
    {
        event->accept();
        scene()->clearSelection();
        setSelected(true);
    }
    else
    {
        event->ignore();
        QGraphicsItem::mousePressEvent(event);
    }
}

void EdgeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && _hover)
    {
        event->accept();
        EditEdgeDialog dialog;
        dialog.exec();
    }
    else
        QGraphicsItem::mousePressEvent(event);
}

void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

}
