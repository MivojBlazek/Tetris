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
     setRect(5 * CELL_SIZE + MARGIN / 2, 0 + MARGIN / 2, BLOCK_SIZE, BLOCK_SIZE);
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

void Block::moveDown()
{
    setPos(pos() + QPointF(0, CELL_SIZE));
}

void Block::changeColor(QColor color)
{
    mColor = color;
    update();
}
