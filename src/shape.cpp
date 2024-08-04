#include "shape.h"

#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QColor>

Shape::Shape(ShapeType type, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent),
      mType(type)
{
    blocks.append(new Block(this));
    blocks.append(new Block(this));
    blocks.append(new Block(this));
    blocks.append(new Block(this));

    // Arrange blocks
    blocks[0]->setPos(0, 0);
    switch (type)
    {
        case I:
            blocks[1]->setPos(-40, 0);
            blocks[2]->setPos(-80, 0);
            blocks[3]->setPos(40, 0);
            break;
        case O:
            blocks[1]->setPos(-40, 0);
            blocks[2]->setPos(0, 40);
            blocks[3]->setPos(-40, 40);
            break;
        case T:
            blocks[0]->setPos(-40, 0);
            blocks[1]->setPos(-40, 40);
            blocks[2]->setPos(-80, 40);
            blocks[3]->setPos(0, 40);
            break;
        case S:
            blocks[1]->setPos(-40, 0);
            blocks[2]->setPos(-80, 40);
            blocks[3]->setPos(-40, 40);
            break;
        case Z:
            blocks[0]->setPos(-40, 0);
            blocks[1]->setPos(-80, 0);
            blocks[2]->setPos(-40, 40);
            blocks[3]->setPos(0, 40);
            break;
        case J:
            blocks[0]->setPos(-80, 0);
            blocks[1]->setPos(-80, 40);
            blocks[2]->setPos(-40, 40);
            blocks[3]->setPos(0, 40);
            break;
        case L:
            blocks[1]->setPos(0, 40);
            blocks[2]->setPos(-40, 40);
            blocks[3]->setPos(-80, 40);
            break;
    }

    // Add blocks to the group
    for (Block *block : blocks)
    {
        addToGroup(block);
    }
}

QList<Block *> Shape::getBlocks() const
{
    return blocks;
}

void Shape::moveDown(qreal step)
{
    setPos(pos() + QPointF(0, step));
}

void Shape::moveLeft(qreal step)
{
    setPos(pos() + QPointF(-step, 0));
}

void Shape::moveRight(qreal step)
{
    setPos(pos() + QPointF(step, 0));
}

void Shape::changeColor(QColor color)
{
    for (Block *block : blocks)
    {
        block->changeColor(color);
        update();
    }
}

void Shape::rotate()
{
    //TODO
}
