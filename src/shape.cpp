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

    // Arrange blocks (blocks[0] will be pivot during rotations)
    arrangeBlocks();

    // Add blocks to the group
    for (Block *block : blocks)
    {
        addToGroup(block);
    }
}

void Shape::arrangeBlocks()
{
    switch (mType)
    {
        case I:
            blocks[0]->setPos(-40, 0);
            blocks[1]->setPos(-80, 0);
            blocks[2]->setPos(0, 0);
            blocks[3]->setPos(40, 0);
            changeColor(Qt::cyan);
            break;
        case O:
            blocks[0]->setPos(0, 0);
            blocks[1]->setPos(-40, 0);
            blocks[2]->setPos(0, 40);
            blocks[3]->setPos(-40, 40);
            changeColor(Qt::yellow);
            break;
        case T:
            blocks[0]->setPos(-40, 40);
            blocks[1]->setPos(0, 40);
            blocks[2]->setPos(-80, 40);
            blocks[3]->setPos(-40, 0);
            changeColor(Qt::magenta);
            break;
        case S:
            blocks[0]->setPos(-40, 0);
            blocks[1]->setPos(0, 0);
            blocks[2]->setPos(-80, 40);
            blocks[3]->setPos(-40, 40);
            changeColor(Qt::green);
            break;
        case Z:
            blocks[0]->setPos(-40, 0);
            blocks[1]->setPos(-80, 0);
            blocks[2]->setPos(-40, 40);
            blocks[3]->setPos(0, 40);
            changeColor(Qt::red);
            break;
        case J:
            blocks[0]->setPos(-40, 40);
            blocks[1]->setPos(-80, 40);
            blocks[2]->setPos(0, 40);
            blocks[3]->setPos(-80, 0);
            changeColor(Qt::blue);
            break;
        case L:
            blocks[0]->setPos(-40, 40);
            blocks[1]->setPos(0, 40);
            blocks[2]->setPos(-80, 40);
            blocks[3]->setPos(0, 0);
            changeColor(QColor(255, 127, 0)); // Orange
            break;
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

void Shape::moveUp(qreal step)
{
    setPos(pos() + QPointF(0, -step));
}

void Shape::changeColor(QColor color)
{
    for (Block *block : blocks)
    {
        block->changeColor(color);
        update();
    }
}

void Shape::rotateBackwards()
{
    if (mType == ShapeType::O)
    {
        return;
    }

    QPointF pivotBlock = blocks[0]->pos();
    for (Block *block : blocks)
    {
        QPointF relativePos = block->pos() - pivotBlock;

        qreal newX = -relativePos.y();
        qreal newY = relativePos.x();

        block->setPos(pivotBlock + QPointF(newX, newY));
    }
}

void Shape::rotate()
{
    if (mType == ShapeType::O)
    {
        return;
    }

    QPointF pivotBlock = blocks[0]->pos();
    for (Block *block : blocks)
    {
        QPointF relativePos = block->pos() - pivotBlock;

        qreal newX = relativePos.y();
        qreal newY = -relativePos.x();

        block->setPos(pivotBlock + QPointF(newX, newY));
    }
}

Shape::ShapeType Shape::getShapeType() const
{
    return mType;
}
