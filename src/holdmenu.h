#ifndef HOLDMENU_H
#define HOLDMENU_H

#include "shape.h"

#include <QGraphicsScene>
#include <QWidget>

namespace Ui {
class HoldMenu;
}

class HoldMenu : public QWidget
{
    Q_OBJECT

public:
    explicit HoldMenu(QWidget *parent = nullptr);
    ~HoldMenu();

public slots:
    void setHold(Shape *shape);

private:
    Ui::HoldMenu *ui;

    QGraphicsScene *previewScene;
};

#endif // HOLDMENU_H
