#ifndef BLOCK_H
#define BLOCK_H

#define MARGIN 2

#include <QGraphicsRectItem>
#include <QObject>
#include <QColor>

class Block : public QGraphicsRectItem
{
public:
    explicit Block(QGraphicsItem *parent = nullptr);

    void moveDown();
    void changeColor(QColor color);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor mColor;
};

#endif // BLOCK_H
