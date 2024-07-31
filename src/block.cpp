#include "block.h"

#include <QGraphicsRectItem>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QColor>

Block::Block(QGraphicsItem *parent)
    : QGraphicsRectItem{parent},
      mColor{QColor(150, 150, 150, 128)}
{
     setRect(200 + MARGIN / 2, 0 + MARGIN / 2, 40 - MARGIN, 40 - MARGIN);
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black));
    painter->drawRect(boundingRect());

    QColor fillColor = mColor;
    painter->fillRect(boundingRect(), fillColor);
}

int Block::moveDown(qreal step)
{
    if (scenePos().y() < 760.0)
    {
        setPos(pos() + QPointF(0, step));
    }
    else
    {
        return 1;
    }
    return 0;
}

void Block::moveLeft(qreal step)
{
    if (scenePos().x() + 200 > 0.0 && scenePos().y() < 760.0)
    {
        setPos(pos() + QPointF(-step, 0));
    }
}

void Block::moveRight(qreal step)
{
    if (scenePos().x() + 200 < 360.0 && scenePos().y() < 760.0)
    {
        setPos(pos() + QPointF(step, 0));
    }
}

void Block::changeColor(QColor color)
{
    mColor = color;
    update();
}
