#include "scene.h"
#include "shape.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>
#include <QKeyEvent>

Scene::Scene(QObject *parent)
    : QGraphicsScene{parent},
      isDropping{false}
{
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &Scene::timeout);

    // Rows for detection full rows
    for (int positionFromTop = 0; positionFromTop < 800; positionFromTop += 40)
    {
        rows.append(new QGraphicsLineItem(0, positionFromTop + 20, 400, positionFromTop + 20));
    }

    // Borders of graphics view
    borders.append(new QGraphicsLineItem(0, 0, 0, 800));
    borders.append(new QGraphicsLineItem(0, 0, 400, 0));
    borders.append(new QGraphicsLineItem(0, 800, 400, 800));
    borders.append(new QGraphicsLineItem(400, 0, 400, 800));
}

void Scene::start()
{
    checkFullRows();
    mShape = new Shape(nextType());
    this->addItem(mShape);

    mTimer->start(FALLING_SPEED);
    isDropping = true;

    this->setFocus();
}

void Scene::stop()
{
    mTimer->stop();
    for (Block *block : mShape->getBlocks())
    {
        blocks.append(block);
    }
    isDropping = false;
}

void Scene::timeout()
{
    if (!isCollision(DOWN))
    {
        mShape->moveDown();
    }
    else
    {
        // Save to blocks and send next shape
        for (Block *block : mShape->getBlocks())
        {
            blocks.append(block);
        }

        // If we lost game
        if (mShape->scenePos().y() <= 0.0)
        {
            mShape->changeColor(Qt::red);
            this->stop();
            return;
        }

        checkFullRows();
        mShape = new Shape(nextType());
        this->addItem(mShape);
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
                mShape->moveDown();
            }
        }
        else if (event->key() == Qt::Key_Left)
        {
            if (!isCollision(LEFT))
            {
                mShape->moveLeft();
            }
        }
        else if (event->key() == Qt::Key_Right)
        {
            if (!isCollision(RIGHT))
            {
                mShape->moveRight();
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

    // Check collisions with borders
    for (auto border : borders)
    {
        if (mShape->collidesWithItem(border))
        {
            smallMoveToCollisionDetection(direction, -5.0);
            return true;
        }
    }

    // Check collisions with other blocks
    for (Block *block : mShape->getBlocks())
    {
        for (Block *otherBlock : blocks)
        {
            if (block->collidesWithItem(otherBlock))
            {
                smallMoveToCollisionDetection(direction, -5.0);
                return true;
            }
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
            mShape->moveDown(step);
            break;
        case LEFT:
            mShape->moveLeft(step);
            break;
        case RIGHT:
            mShape->moveRight(step);
            break;
    }
}

Shape::ShapeType Scene::nextType()
{
    int randomInt = QRandomGenerator::global()->bounded(0, 7);
    return static_cast<Shape::ShapeType>(randomInt);
}

void Scene::checkFullRows()
{
    QList<Block *> blocksToRemove;
    int blocksInRow;
    for (auto row : rows)
    {
        blocksToRemove.clear();
        blocksInRow = 0;

        for (Block *block : blocks)
        {
            if (row->collidesWithItem(block))
            {
                // If block collides with selected row
                blocksToRemove.append(block);
                blocksInRow++;
            }
        }
        if (blocksInRow == 10)
        {
            // Clear this row
            clearRow(blocksToRemove);
            shiftRowsDown(row->line().y1());
        }
    }
}

void Scene::clearRow(QList<Block *> blocksToDelete)
{
    for (Block *block : blocksToDelete)
    {
        blocks.removeOne(block);
        this->removeItem(block);
        delete block;
    }
}

void Scene::shiftRowsDown(qreal deletedRowPosition)
{
    for (Block *block : blocks)
    {
        // Shift all blocks above deleted row
        if (block->scenePos().y() < deletedRowPosition - 20.0)
        {
            block->moveDown();
        }
    }
}
