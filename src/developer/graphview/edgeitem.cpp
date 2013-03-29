/*!
 * \file
 */
#include "edgeitem.hpp"
#include "edge.hpp"

#include <QApplication>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QSettings>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

namespace Developer {

EdgeItem::EdgeItem(Edge *edge, NodeItem *edgeFrom, NodeItem *edgeTo,
                   QGraphicsItem *parent)
    : GraphItem(edge->id(), edge->label(), parent)
    , _from(edgeFrom)
    , _to(edgeTo)
{
    setZValue(EDGE_Z_VALUE);
}

EdgeItem::EdgeItem(const QString &edgeId, NodeItem *edgeFrom, NodeItem *edgeTo,
                   const QString &edgeLabel, QGraphicsItem *parent)
    : GraphItem(edgeId, edgeLabel,parent)
    , _from(edgeFrom)
    , _to(edgeTo)
{
    setZValue(EDGE_Z_VALUE);
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
}

void EdgeItem::setTo(NodeItem *edgeTo)
{
    _to = edgeTo;
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

QRectF EdgeItem::boundingRect() const
{
    QLineF edgeLine = line();

    return QRectF(edgeLine.p1(), edgeLine.p2());
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(widget)

    QSettings settings;
    QColor lineColour = settings.value("GraphView/Edges/LineColour",
                                       QColor(0x33,0x33,0x33)).value<QColor>();
    qreal lineWidth = settings.value("GraphView/Edges/LineWidth", 1.5).toDouble();
    qreal arrowSize = settings.value("GraphView/Edges/ArrowSize", 9).toDouble();

    if(SHOW_VISUALISATION_DEBUG)
    {
        painter->setPen(DEBUG_COLOUR);
        painter->drawRect(boundingRect());
    }

    QPen pen(lineColour);
    pen.setWidth(lineWidth);
    painter->setPen(pen);
    painter->setBrush(lineColour);

    QLineF edgeLine = line();
    // Compensate for pen width
    edgeLine.setLength(edgeLine.length()-(lineWidth+0.5));
    QPainterPath path(edgeLine.p1());
    path.lineTo(edgeLine.p2());

    // Abuse QLineF's ability to transform lines in order to draw a triangle
    // at the end of this edge as follows:

    // Get the current angle
    qreal angle = edgeLine.angle();
    // Shift it around by 150 degrees, this leaves 30 degrees as the internal
    // angle which is what we want as the edge cuts the equilateral triangle's
    // 60 degree internal angles in half
    // If we overshoot and get negative simply add 360 to fix it
    angle -= 150; if(angle < 0) angle += 360;
    // Move the line such that p1 is now where p2 sat
    edgeLine.translate(edgeLine.p2()-edgeLine.p1());
    // Resize the line to be the length of an edge of the triangle
    edgeLine.setLength(arrowSize);
    // Set the new angle, p2 is now in the correct place
    edgeLine.setAngle(angle);
    // Draw a line to this point
    path.lineTo(edgeLine.p2());

    // Now that we've done the initial line the rest are all relative to the
    // equilateral triangle, and therefore we are moving to produce 60 degree
    // internal angles
    angle -= 120; if(angle < 0) angle += 360;
    // The rest is exactly the same
    edgeLine.translate(edgeLine.p2()-edgeLine.p1());
    edgeLine.setLength(arrowSize);
    edgeLine.setAngle(angle);
    path.lineTo(edgeLine.p2());

    // One last time to get us back to our origin
    angle -= 120; if(angle < 0) angle += 360;
    edgeLine.translate(edgeLine.p2()-edgeLine.p1());
    edgeLine.setLength(arrowSize);
    edgeLine.setAngle(angle);
    path.lineTo(edgeLine.p2());

    // Actually draw it
    painter->drawPath(path);
}

}
