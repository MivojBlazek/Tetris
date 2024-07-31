#ifndef SCENE_H
#define SCENE_H

#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define FALLING_SPEED 1000

#include "block.h"

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

    QTimer *mTimer;
    Block *mBlock;
    bool isDropping;

    bool isCollision(int direction = DOWN);
    void smallMoveToCollisionDetection(int direction, qreal step);

    QList<Block *> blocks;
};

#endif // SCENE_H
