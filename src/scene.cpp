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

    mTimer->start(1000);
    isDropping = true;

    this->setFocus();
}

void Scene::stop()
{
    mTimer->stop();
    isDropping = false;
}

void Scene::timeout()
{
    mBlock->moveDown();
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if (isDropping)
    {
        if (event->key() == Qt::Key_Left)
        {
            mBlock->moveLeft();
        }
        else if (event->key() == Qt::Key_Right)
        {
            mBlock->moveRight();
        }
        else
        {
            QGraphicsScene::keyPressEvent(event);
        }
    }
}
