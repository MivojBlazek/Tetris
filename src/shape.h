#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItemGroup>
#include <QList>

#include "block.h"

class Shape : public QGraphicsItemGroup
{
public:
    enum ShapeType { I, O, T, S, Z, J, L };
    enum CollisionDirection { NONE, DOWN, LEFT, RIGHT, UP };

    explicit Shape(ShapeType type, QGraphicsItem *parent = nullptr);

    QList<Block *> getBlocks() const;
    Shape::ShapeType getShapeType() const;

    void moveDown(qreal step = 40.0);
    void moveLeft(qreal step = 40.0);
    void moveRight(qreal step = 40.0);
    void moveUp(qreal step = 40.0);
    void changeColor(QColor color);
    void rotate();
    void rotateBackwards();

private:
    void arrangeBlocks();

    QList<Block *> blocks;
    ShapeType mType;
};

#endif // SHAPE_H
