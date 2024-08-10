#ifndef SCENE_H
#define SCENE_H

#include "config.h"

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
    enum ScoreType { SOFT_DROP, HARD_DROP, ONE_ROW, TWO_ROWS, THREE_ROWS, FOUR_ROWS };

    void startButtonPressed();
    void start();
    void stop();
    void setSpeed(int speed);

    void newState();

signals:
    void nextShapeGenerated(Shape *nextShape);
    void addedToHold(Shape *holdShape);
    void addScore(QString score);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupGrid();

    void timeout();
    Shape::ShapeType nextType();
    int checkFullRows();
    void clearRow(QList<Block *> blocks);
    void shiftRowsDown(qreal deletedRowPosition);

    void checkCollisions(CollisionDirection directionBorder[], int *numberOfBorderMoves, CollisionDirection *directionBlock, int *numberOfBlockMoves);
    void putRotationBack(CollisionDirection directionBorder[], int numberOfBorderMoves, CollisionDirection directionBlock, int numberOfBlockMoves, bool backwards = true);

    QTimer *mTimer;
    Shape *mShape;
    Shape *nextShape;
    Shape *holdShape;
    bool isDropping;
    bool holdDoneThisRound;

    bool isCollision(int direction = DOWN);
    void smallMoveToCollisionDetection(int direction, qreal step);
    Scene::CollisionDirection isAlreadyBorderCollision();
    Scene::CollisionDirection isAlreadyBlockCollision();

    void addSomeScore(Scene::ScoreType scoreToAdd);

    void updatePreview();
    Shape *preview;
    bool isPreviewDown();

    QList<Block *> blocks;
    QList<QGraphicsRectItem *> borders;
    QList<QGraphicsLineItem *> rows;
    QList<QGraphicsLineItem *> grid;
};

#endif // SCENE_H
