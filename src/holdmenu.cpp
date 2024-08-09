#include "holdmenu.h"
#include "ui_holdmenu.h"
#include "scene.h"

HoldMenu::HoldMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HoldMenu),
    previewScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(previewScene);
    ui->graphicsView->setFixedSize(5 * CELL_SIZE + 2, 5 * CELL_SIZE + 2);

    QGraphicsView *view = ui->graphicsView;
    view->setStyleSheet("background-color: white; border: 1px solid black;");
}

HoldMenu::~HoldMenu()
{
    delete ui;
}


void HoldMenu::setHold(Shape *shape)
{
    previewScene->clear();
    shape->setPos(
                     (ui->graphicsView->width() - shape->boundingRect().width()) / 2,
                     (ui->graphicsView->height() - shape->boundingRect().height()) / 2
                 );
    previewScene->addItem(shape);
}
