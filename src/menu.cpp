#include "menu.h"
#include "ui_menu.h"
#include "scene.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    previewScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(previewScene);

    connect(ui->StartButton, &QPushButton::clicked, this, &Menu::onStartButtonClicked);

    ui->StartButton->show();
}

Menu::~Menu()
{
    delete ui;
}

void Menu::onStartButtonClicked()
{
    emit start();
    ui->StartButton->hide();
}

void Menu::setNextShape(Shape *shape)
{
    previewScene->clear();
    shape->setPos(
                     (ui->graphicsView->width() - shape->boundingRect().width()) / 2,
                     (ui->graphicsView->height() - shape->boundingRect().height()) / 2
                 );
    previewScene->addItem(shape);
}
