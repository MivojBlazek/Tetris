#ifndef SHAPE_H
#define SHAPE_H

#include "config.h"

#include "block.h"

#include <QGraphicsItemGroup>
#include <QList>

class Shape : public QGraphicsItemGroup
{
public:
    enum ShapeType { NO_SHAPE, I, O, T, S, Z, J, L };
    enum CollisionDirection { NONE, DOWN, LEFT, RIGHT, UP };

    explicit Shape(ShapeType type, QGraphicsItem *parent = nullptr);

    QList<Block *> getBlocks() const;
    Shape::ShapeType getShapeType() const;

    void moveDown(qreal step = CELL_SIZE);
    void moveLeft(qreal step = CELL_SIZE);
    void moveRight(qreal step = CELL_SIZE);
    void moveUp(qreal step = CELL_SIZE);
    void changeColor(QColor color);
    void rotate();
    void rotateBackwards();

private:
    void arrangeBlocks();

    QList<Block *> blocks;
    ShapeType mType;
};

#endif // SHAPE_H
