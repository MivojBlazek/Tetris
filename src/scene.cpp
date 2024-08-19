#include "config.h"

#ifdef AI
#include "../../Tetris-AI/src/gamestate.h"
#include "../../Tetris-AI/src/artificialintelligence.h"
#include "../../Tetris-AI/src/outcome.h"
#endif

#include "scene.h"
#include "shape.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>
#include <QKeyEvent>

Scene::Scene(QObject *parent)
    : QGraphicsScene{parent},
      isDropping{false},
      holdDoneThisRound{false},
      preview{nullptr}
{
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &Scene::timeout);

    // Rows for detection full rows
    for (int positionFromTop = 0; positionFromTop < 20 * CELL_SIZE; positionFromTop += CELL_SIZE)
    {
        rows.append(new QGraphicsLineItem(0, positionFromTop + CELL_SIZE / 2, 10 * CELL_SIZE, positionFromTop + CELL_SIZE / 2));
    }

    // Borders of graphics view
    borders.append(new QGraphicsRectItem(0, 0, 0 - SIZE_OUT_OF_MAP, 20 * CELL_SIZE));                           // Left
    borders.append(new QGraphicsRectItem(0, 20 * CELL_SIZE, 10 * CELL_SIZE + SIZE_OUT_OF_MAP, 20 * CELL_SIZE)); // Down
    borders.append(new QGraphicsRectItem(10 * CELL_SIZE, 0, 10 * CELL_SIZE, 20 * CELL_SIZE + SIZE_OUT_OF_MAP)); // Right

    setupGrid();
}

void Scene::startButtonPressed()
{
    nextShape = new Shape(nextType());
    holdShape = nullptr;
    mShape = nullptr;
    emit nextShapeGenerated(nextShape);
    start();
}

void Scene::setupGrid()
{
    for (int positionFromTop = CELL_SIZE; positionFromTop < 20 * CELL_SIZE; positionFromTop += CELL_SIZE)
    {
        grid.append(new QGraphicsLineItem(0, positionFromTop, 10 * CELL_SIZE, positionFromTop));
    }
    for (int positionFromLeft = CELL_SIZE; positionFromLeft < 10 * CELL_SIZE; positionFromLeft += CELL_SIZE)
    {
        grid.append(new QGraphicsLineItem(positionFromLeft, 0, positionFromLeft, 20 * CELL_SIZE));
    }
    for (auto line : grid)
    {
        line->setPen(QPen(QColor(127, 127, 127, 100)));
        this->addItem(line);
    }
}

void Scene::start()
{
    checkFullRows();
    if (preview && preview != nullptr)
    {
        delete preview;
    }
    mShape = new Shape(nextShape->getShapeType());
    preview = new Shape(nextShape->getShapeType());
    updatePreview();
    delete nextShape;

    nextShape = new Shape(nextType());
    emit nextShapeGenerated(nextShape);
    holdDoneThisRound = false;

    this->addItem(mShape);
    this->addItem(preview);

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

void Scene::setSpeed(int speed)
{
    mTimer->start(speed);
}

void Scene::timeout()
{
    if (!isCollision(DOWN, mShape, blocks))
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
            mShape->changeColor(Qt::gray);
            this->stop();
            return;
        }

        checkFullRows();
        if (preview && preview != nullptr)
        {
            delete preview;
        }
        mShape = new Shape(nextShape->getShapeType());
        preview = new Shape(nextShape->getShapeType());
        updatePreview();
        delete nextShape;

        nextShape = new Shape(nextType());
        emit nextShapeGenerated(nextShape);
        holdDoneThisRound = false;

        this->addItem(mShape);
        this->addItem(preview);
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
            if (!isCollision(DOWN, mShape, blocks))
            {
                addSomeScore(SOFT_DROP);
                mShape->moveDown();
            }
        }
        else if (event->key() == Qt::Key_Space)
        {
            while (!isCollision(DOWN, mShape, blocks))
            {
                addSomeScore(HARD_DROP);
                mShape->moveDown();
            }
            timeout();
        }
        else if (event->key() == Qt::Key_Left)
        {
            if (!isCollision(LEFT, mShape, blocks))
            {
                mShape->moveLeft();
                preview->moveLeft();
                updatePreview();
            }
        }
        else if (event->key() == Qt::Key_Right)
        {
            if (!isCollision(RIGHT, mShape, blocks))
            {
                mShape->moveRight();
                preview->moveRight();
                updatePreview();
            }
        }
        else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_X)
        {
            mShape->rotateBackwards();
            preview->rotateBackwards();
            updatePreview();
            checkCollisions(directionBorder, &numberOfBorderMoves, &directionBlock, &numberOfBlockMoves);

            // If we are stuck after rotation, put all moves back
            putRotationBack(directionBorder, numberOfBorderMoves, directionBlock, numberOfBlockMoves, false);
        }
        else if (event->key() == Qt::Key_Z)
        {
            mShape->rotate();
            preview->rotate();
            updatePreview();
            checkCollisions(directionBorder, &numberOfBorderMoves, &directionBlock, &numberOfBlockMoves);

            // If we are stuck after rotation, put all moves back
            putRotationBack(directionBorder, numberOfBorderMoves, directionBlock, numberOfBlockMoves);
        }
        else if (event->key() == Qt::Key_C)
        {
            // User can use hold only once per piece drop
            if (!holdDoneThisRound)
            {
                holdDoneThisRound = true;

                Shape *tmpShape = nullptr;
                if (holdShape != nullptr)
                {
                    // Take shape from hold
                    tmpShape = new Shape(holdShape->getShapeType());
                    delete holdShape;
                    holdShape = nullptr;
                }
                // Add to hold
                holdShape = new Shape(mShape->getShapeType());
                emit addedToHold(holdShape);
                delete mShape;
                mShape = nullptr;
                delete preview;
                preview = nullptr;

                // Change actual shape on saved one from hold
                if (tmpShape != nullptr)
                {
                    mShape = new Shape(tmpShape->getShapeType());
                    preview = new Shape(tmpShape->getShapeType());
                    updatePreview();
                    delete tmpShape;

                    this->addItem(mShape);
                    this->addItem(preview);
                }
                else
                {
                    mShape = new Shape(nextShape->getShapeType());
                    preview = new Shape(nextShape->getShapeType());
                    updatePreview();
                    delete nextShape;

                    nextShape = new Shape(nextType());
                    emit nextShapeGenerated(nextShape);

                    this->addItem(mShape);
                    this->addItem(preview);
                }
            }
        }
        else
        {
            QGraphicsScene::keyPressEvent(event);
        }
        updatePreview();
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

bool Scene::isCollision(int direction, Shape *shape, QList<Block *> allBLocks)
{
    smallMoveToCollisionDetection(direction, shape, 5.0);

    // Check collisions with borders
    for (auto border : borders)
    {
        for (Block *block : shape->getBlocks())
        {
            if (block->collidesWithItem(border))
            {
                smallMoveToCollisionDetection(direction, shape, -5.0);
                return true;
            }
        }
    }

    // Check collisions with other blocks
    for (Block *block : shape->getBlocks())
    {
        for (Block *otherBlock : allBLocks)
        {
            if (block->collidesWithItem(otherBlock))
            {
                smallMoveToCollisionDetection(direction, shape, -5.0);
                return true;
            }
        }
    }

    smallMoveToCollisionDetection(direction, shape, -5.0);
    return false;
}

void Scene::smallMoveToCollisionDetection(int direction, Shape *shape, qreal step)
{
    switch (direction)
    {
        case DOWN:
            shape->moveDown(step);
            break;
        case LEFT:
            shape->moveLeft(step);
            break;
        case RIGHT:
            shape->moveRight(step);
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
                if (border == borders[1]) return DOWN;
                if (border == borders[2]) return RIGHT;
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
    int randomInt = QRandomGenerator::global()->bounded(1, 8);
    return static_cast<Shape::ShapeType>(randomInt);
}

int Scene::checkFullRows()
{
    int numberOfRemovedRows = 0;

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

            numberOfRemovedRows++;
        }
    }

    // How many rows were removed
    switch (numberOfRemovedRows)
    {
        case 1:
            addSomeScore(ONE_ROW);
            break;
        case 2:
            addSomeScore(TWO_ROWS);
            break;
        case 3:
            addSomeScore(THREE_ROWS);
            break;
        case 4:
            addSomeScore(FOUR_ROWS);
            break;
        default:
            break;
    }
    return numberOfRemovedRows;
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
        if (block->scenePos().y() < deletedRowPosition - CELL_SIZE / 2)
        {
            block->moveDown();
        }
    }
}

void Scene::addSomeScore(ScoreType scoreToAdd)
{
    switch (scoreToAdd)
    {
        case SOFT_DROP:
            emit addScore("1");
            break;
        case HARD_DROP:
            emit addScore("2");
            break;
        case ONE_ROW:
            emit addScore("100");
            break;
        case TWO_ROWS:
            emit addScore("300");
            break;
        case THREE_ROWS:
            emit addScore("500");
            break;
        case FOUR_ROWS:
            emit addScore("800");
            break;
        default:
            break;
    }
}

void Scene::updatePreview()
{
    preview->setPos(mShape->pos().x(), 0.0);
    preview->changeColor(QColor(127, 127, 127, 40));
    while (!isPreviewDown())
    {
        preview->moveDown();
    }
}

bool Scene::isPreviewDown()
{
    preview->moveDown(5);
    for (Block *previewBlock : preview->getBlocks())
    {
        if (previewBlock->collidesWithItem(borders[1]))
        {
            preview->moveDown(-5);
            return true;
        }
        for (Block *fallenBlock : blocks)
        {
            if (previewBlock->collidesWithItem(fallenBlock))
            {
                preview->moveDown(-5);
                return true;
            }
        }
    }
    preview->moveDown(-5);
    return false;
}

QList<QGraphicsLineItem *> Scene::getRows()
{
    return rows;
}

#ifdef AI
void Scene::newState()
{
    GameState state(blocks, mShape, nextShape, holdShape);
    ArtificialIntelligence *ai = new ArtificialIntelligence(this);
    Outcome bestOutcome = ai->findBestOutcome(state);
    ai->movePiece(bestOutcome, mShape, preview);

    if (preview && preview != nullptr)
    {
        updatePreview();
    }

    if (mShape && mShape != nullptr)
    {
        QTimer::singleShot(200, this, [=]()
        {
            // Perform hard drop
            while (!isCollision(DOWN, mShape, blocks))
            {
                addSomeScore(HARD_DROP);
                mShape->moveDown();
            }
            timeout();
        });
    }
}
#endif
