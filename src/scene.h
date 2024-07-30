#ifndef SCENE_H
#define SCENE_H

#include "block.h"

#include <QGraphicsScene>

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

};

#endif // SCENE_H
