#include "scene.h"
#include "shape.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>
#include <QKeyEvent>
#include <memory>

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
    borders.append(new QGraphicsRectItem(0, 0, 0 - SIZE_OUT_OF_MAP, 800));        // Left
    borders.append(new QGraphicsRectItem(0, 0, 400, 0 - SIZE_OUT_OF_MAP));        // Up
    borders.append(new QGraphicsRectItem(0, 800, 400 + SIZE_OUT_OF_MAP, 800));    // Down
    borders.append(new QGraphicsRectItem(400, 0, 400, 800 + SIZE_OUT_OF_MAP));    // Right
}

void Scene::startButtonPressed()
{
    nextShape = new Shape(nextType());
    emit nextShapeGenerated(nextShape);
    start();
}

void Scene::start()
{
    checkFullRows();
    mShape = new Shape(nextShape->getShapeType());
    delete nextShape;

    nextShape = new Shape(nextType());
    emit nextShapeGenerated(nextShape);

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
        mShape = new Shape(nextShape->getShapeType());
        delete nextShape;

        nextShape = new Shape(nextType());
        emit nextShapeGenerated(nextShape);

        this->addItem(mShape);
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if (isDropping)
    {
        // Variables for rotations
        int numberOfBorderMoves = 0;
        int numberOfBlockMoves = 0;
        CollisionDirection directionBorder[3];
        directionBorder[numberOfBorderMoves] = NONE;
        CollisionDirection directionBlock = NONE;

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
        else if (event->key() == Qt::Key_Up)
        {
            mShape->rotateBackwards();
            checkCollisions(directionBorder, &numberOfBorderMoves, &directionBlock, &numberOfBlockMoves);

            // If we are stuck after rotation, put all moves back
            putRotationBack(directionBorder, numberOfBorderMoves, directionBlock, numberOfBlockMoves, false);
        }
        else if (event->key() == Qt::Key_Z)
        {
            mShape->rotate();
            checkCollisions(directionBorder, &numberOfBorderMoves, &directionBlock, &numberOfBlockMoves);

            // If we are stuck after rotation, put all moves back
            putRotationBack(directionBorder, numberOfBorderMoves, directionBlock, numberOfBlockMoves);
        }
        else
        {
            QGraphicsScene::keyPressEvent(event);
        }
    }
}

void Scene::checkCollisions(CollisionDirection directionBorder[], int *numberOfBorderMoves, CollisionDirection *directionBlock, int *numberOfBlockMoves)
{
    // Are there collisions with border? Max 2 if shape I is there
    while ((directionBorder[*numberOfBorderMoves] = isAlreadyBorderCollision()) != NONE)
    {
        switch (directionBorder[*numberOfBorderMoves])
        {
            case DOWN:
                mShape->moveUp();
                break;
            case LEFT:
                mShape->moveRight();
                break;
            case RIGHT:
                mShape->moveLeft();
                break;
            case UP:
                mShape->moveDown();
                break;
            default:
                break;
        }
        (*numberOfBorderMoves)++;
    }

    // Are there collisions with block? Max 2 if shape I is there
    *directionBlock = isAlreadyBlockCollision();
    do
    {
        (*numberOfBlockMoves)++;
        switch (*directionBlock)
        {
            case DOWN:
                mShape->moveUp();
                break;
            case LEFT:
                mShape->moveRight();
                break;
            case RIGHT:
                mShape->moveLeft();
                break;
            default:
                break;
        }
    } while (mShape->getShapeType() == Shape::ShapeType::I && isAlreadyBlockCollision() && *numberOfBlockMoves == 1);
}

void Scene::putRotationBack(CollisionDirection directionBorder[], int numberOfBorderMoves, CollisionDirection directionBlock, int numberOfBlockMoves, bool backwards)
{
    // Are there still collisions
    if (isAlreadyBorderCollision() || isAlreadyBlockCollision())
    {
        // Rotate back
        if (backwards)
        {
            mShape->rotateBackwards();
        }
        else
        {
            mShape->rotate();
        }

        // Move back to position before border collisions
        for (int i = numberOfBorderMoves; i >= 0; i--)
        {
            switch (directionBorder[i])
            {
                case DOWN:
                    mShape->moveDown();
                    break;
                case LEFT:
                    mShape->moveLeft();
                    break;
                case RIGHT:
                    mShape->moveRight();
                    break;
                case UP:
                    mShape->moveUp();
                    break;
                default:
                    break;
            }
        }

        // Move back to position before block collisions
        for (int i = numberOfBlockMoves; i > 0; i--)
        {
            switch (directionBlock)
            {
                case DOWN:
                    mShape->moveDown();
                    break;
                case LEFT:
                    mShape->moveLeft();
                    break;
                case RIGHT:
                    mShape->moveRight();
                    break;
                default:
                    break;
            }
        }
    }
}

bool Scene::isCollision(int direction)
{
    smallMoveToCollisionDetection(direction, 5.0);

    // Check collisions with borders
    for (auto border : borders)
    {
        for (Block *block : mShape->getBlocks())
        {
            if (block->collidesWithItem(border))
            {
                smallMoveToCollisionDetection(direction, -5.0);
                return true;
            }
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
        default:
            break;
    }
}

Scene::CollisionDirection Scene::isAlreadyBorderCollision()
{
    // Check collisions with borders
    for (auto border : borders)
    {
        for (Block *block : mShape->getBlocks())
        {
            if (block->collidesWithItem(border))
            {
                if (border == borders[0]) return LEFT;
                if (border == borders[1]) return UP;
                if (border == borders[2]) return DOWN;
                if (border == borders[3]) return RIGHT;
            }
        }
    }
    return NONE;
}

Scene::CollisionDirection Scene::isAlreadyBlockCollision()
{
    // Check collisions with other blocks
    for (Block *block : mShape->getBlocks())
    {
        for (Block *otherBlock : blocks)
        {
            if (block->collidesWithItem(otherBlock))
            {
                qreal dx = mShape->getBlocks()[0]->scenePos().x() - otherBlock->scenePos().x();
                qreal dy = mShape->getBlocks()[0]->scenePos().y() - otherBlock->scenePos().y();

                // If difference on y-axis is greater, collision is under falling piece
                if (qAbs(dx) < qAbs(dy))
                {
                    if (dy < 0)
                    {
                        return DOWN;
                    }
                }
                else
                {
                    // Determine which side collision is
                    if (dx > 0)
                    {
                        return LEFT;
                    }
                    else
                    {
                        return RIGHT;
                    }
                }
            }
        }
    }
    return NONE;
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
        // After 10 collisions (10 blocks in a row)
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
