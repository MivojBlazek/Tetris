#include "block.h"

#include <QGraphicsRectItem>
#include <QPainter>
#include <QDebug>
#include <QPen>

Block::Block(QGraphicsItem *parent)
    : QGraphicsRectItem{parent}
{
     setRect(200, 0, 40, 40);
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black));
    painter->drawRect(boundingRect());

    QColor fillColor = QColor(150, 150, 150, 128);
    painter->fillRect(boundingRect(), fillColor);
}

void Block::moveDown()
{
    if (scenePos().y() < 760.0)
    {
        setPos(pos() + QPointF(0, 40));
    }
}

void Block::moveLeft()
{
    if (scenePos().x() + 200 > 0.0 && scenePos().y() < 760.0)
    {
        setPos(pos() + QPointF(-40, 0));
    }
}

void Block::moveRight()
{
    if (scenePos().x() + 200 < 360.0 && scenePos().y() < 760.0)
    {
        setPos(pos() + QPointF(40, 0));
    }
}
