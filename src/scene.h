﻿#ifndef SCENE_H
#define SCENE_H

#define FALLING_SPEED 1000
#define SIZE_OUT_OF_MAP 10

#include "shape.h"

#include <QGraphicsScene>
#include <QList>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    enum CollisionDirection { NONE, DOWN, LEFT, RIGHT, UP };

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

    void checkCollisions(CollisionDirection directionBorder[], int *numberOfBorderMoves, CollisionDirection *directionBlock, int *numberOfBlockMoves);
    void putRotationBack(CollisionDirection directionBorder[], int numberOfBorderMoves, CollisionDirection directionBlock, int numberOfBlockMoves, bool backwards = true);

    QTimer *mTimer;
    Shape *mShape;
    bool isDropping;

    bool isCollision(int direction = DOWN);
    void smallMoveToCollisionDetection(int direction, qreal step);
    Scene::CollisionDirection isAlreadyBorderCollision();
    Scene::CollisionDirection isAlreadyBlockCollision();

    QList<Block *> blocks;
    QList<QGraphicsRectItem *> borders;
    QList<QGraphicsLineItem *> rows;
};

#endif // SCENE_H
