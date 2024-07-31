#include "scene.h"
#include "block.h"

#include <QDebug>
#include <QTimer>
#include <QKeyEvent>

Scene::Scene(QObject *parent)
    : QGraphicsScene{parent},
      isDropping{false}
{
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &Scene::timeout);
}

void Scene::start()
{
    mBlock = new Block();
    this->addItem(mBlock);

    mTimer->start(FALLING_SPEED);
    isDropping = true;

    this->setFocus();
}

void Scene::stop()
{
    mTimer->stop();
    blocks.append(mBlock);
    isDropping = false;
}

void Scene::timeout()
{
    if (!isCollision(DOWN))
    {
        if (mBlock->moveDown())
        {
            // save to blocks and send next block
            blocks.append(mBlock);

            mBlock = new Block();
            this->addItem(mBlock);
        }
    }
    else
    {
        // save to blocks and send next block
        blocks.append(mBlock);

        // if we lost game
        if (mBlock->scenePos().y() <= 0.0)
        {
            mBlock->changeColor(Qt::red);
            this->stop();
            return;
        }

        mBlock = new Block();
        this->addItem(mBlock);
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if (isDropping)
    {
        if (event->key() == Qt::Key_Down)
        {
            if (!isCollision(DOWN))
            {
                mBlock->moveDown();
            }
        }
        else if (event->key() == Qt::Key_Left)
        {
            if (!isCollision(LEFT))
            {
                mBlock->moveLeft();
            }
        }
        else if (event->key() == Qt::Key_Right)
        {
            if (!isCollision(RIGHT))
            {
                mBlock->moveRight();
            }
        }
        else
        {
            QGraphicsScene::keyPressEvent(event);
        }
    }
}

bool Scene::isCollision(int direction)
{
    smallMoveToCollisionDetection(direction, 5.0);
    for (Block *block : blocks)
    {
        if (mBlock->collidesWithItem(block) || (direction == DOWN && mBlock->scenePos().y() > 760.0))
        {
            smallMoveToCollisionDetection(direction, -5.0);
            return true;
        }
    }
    smallMoveToCollisionDetection(direction, -5.0);
    return false;
}

void Scene::smallMoveToCollisionDetection(int direction, qreal step)
{
    switch (direction)
    {
        case DOWN:
            mBlock->moveDown(step);
            break;
        case LEFT:
            mBlock->moveLeft(step);
            break;
        case RIGHT:
            mBlock->moveRight(step);
            break;
    }
}
