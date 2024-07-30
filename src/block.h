#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsRectItem>
#include <QObject>

class Block : public QGraphicsRectItem
{
public:
    explicit Block(QGraphicsItem *parent = nullptr);

    void moveDown();
    void moveLeft();
    void moveRight();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};

#endif // BLOCK_H
