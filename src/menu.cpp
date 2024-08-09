#include "menu.h"
#include "ui_menu.h"
#include "scene.h"

#include <QString>

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    previewScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(previewScene);
    ui->graphicsView->setFixedSize(5 * CELL_SIZE + 2, 5 * CELL_SIZE + 2);

    QGraphicsView *view = ui->graphicsView;
    view->setStyleSheet("background-color: white; border: 1px solid black;");

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

void Menu::setFocusToStartButton()
{
    ui->StartButton->setFocus();
    ui->StartButton->setDefault(true);
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

void Menu::setScore(QString score)
{
    ui->scoreLabel->setText(score);
}

void Menu::setHighscore(QString highscore)
{
    ui->highscoreLabel->setText(highscore);
}
