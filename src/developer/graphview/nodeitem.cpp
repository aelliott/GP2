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
}

NodeItem::NodeItem(const QString &nodeId, const QString &nodeLabel, bool root,
                   QGraphicsItem *parent)
    : GraphItem(nodeId,nodeLabel,parent)
    , _shape(Ellipse)
    , _isRoot(root)
{
}

bool NodeItem::isRoot() const
{
    return _isRoot;
}

void NodeItem::setIsRoot(bool root)
{
    _isRoot = root;
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

    QRectF bounds = boundingRect();
    painter->drawEllipse(bounds);

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
