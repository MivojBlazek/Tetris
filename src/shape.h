#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItemGroup>
#include <QList>

#include "block.h"

class Shape : public QGraphicsItemGroup
{
public:
    enum ShapeType { I, O, T, S, Z, J, L };

    explicit Shape(ShapeType type, QGraphicsItem *parent = nullptr);

    QList<Block *> getBlocks() const;

    void moveDown(qreal step = 40.0);
    void moveLeft(qreal step = 40.0);
    void moveRight(qreal step = 40.0);
    void changeColor(QColor color);
    void rotate();

private:
    QList<Block *> blocks;
    ShapeType mType;
};

#endif // SHAPE_H
