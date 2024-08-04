#ifndef SCENE_H
#define SCENE_H

#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define FALLING_SPEED 1000

#include "shape.h"

#include <QGraphicsScene>
#include <QList>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void start();
    void stop();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    void timeout();
    Shape::ShapeType nextType();
    void checkFullRows();
    void clearRow(QList<Block *> blocks);
    void shiftRowsDown(qreal deletedRowPosition);

    QTimer *mTimer;
    Shape *mShape;
    bool isDropping;

    bool isCollision(int direction = DOWN);
    void smallMoveToCollisionDetection(int direction, qreal step);

    QList<Block *> blocks;
    QList<QGraphicsLineItem *> borders;
    QList<QGraphicsLineItem *> rows;
};

#endif // SCENE_H
