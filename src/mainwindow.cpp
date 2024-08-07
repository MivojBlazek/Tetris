#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "menu.h"
#include "holdmenu.h"

#include <QFile>
#include <QString>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      score{"0"}
{
    ui->setupUi(this);
    ui->Right_menu->setFocusToStartButton();
    ui->Right_menu->setScore(score);
    ui->Right_menu->setHighscore(getHighscore());

    resize(850, 850);
    Scene *scene = initScene();

    QGraphicsView *view = ui->graphicsView;
    view->setScene(scene);
    view->setFocusPolicy(Qt::StrongFocus);

    connect(ui->Right_menu, &Menu::start, scene, [scene, view]() {
        scene->startButtonPressed();
        view->setFocus();
    });

    connect(scene, &Scene::nextShapeGenerated, ui->Right_menu, &Menu::setNextShape);
    connect(scene, &Scene::addedToHold, ui->Left_menu, &HoldMenu::setHold);

    connect(scene, &Scene::addScore, this, &MainWindow::addScore);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    exit(0);
}

Scene *MainWindow::initScene()
{
    Scene *scene = new Scene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setSceneRect(0, 0, 400, 800);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setResizeAnchor(QGraphicsView::NoAnchor);
    return scene;
}

void MainWindow::checkHighscore()
{
    QString highscore = getHighscore();
    if (score.toInt() > highscore.toInt())
    {
        setHighscore(score);
    }
}

void MainWindow::addScore(QString addToScore)
{
    score = QString::number(score.toInt() + addToScore.toInt());
    ui->Right_menu->setScore(score);
    increaseSpeed();
    checkHighscore();
}

void MainWindow::increaseSpeed()
{
    Scene *scene = dynamic_cast<Scene *>(ui->graphicsView->scene());
    if (scene)
    {
        int scoreInInteger = score.toInt();
        if (scoreInInteger < 1000) scene->setSpeed(FALLING_SPEED);
        else if (scoreInInteger >= 1000 && scoreInInteger < 2718) scene->setSpeed(FALLING_SPEED / 5 * 4);
        else if (scoreInInteger >= 2718 && scoreInInteger < 7389) scene->setSpeed(FALLING_SPEED / 5 * 3);
        else if (scoreInInteger >= 7389 && scoreInInteger < 20086) scene->setSpeed(FALLING_SPEED / 5 * 2);
        else if (scoreInInteger >= 20086) scene->setSpeed(FALLING_SPEED / 5);
    }
}

QString MainWindow::getHighscore()
{
    QString highscore;

    QFile file(JSON_NAME);
    if (!file.open(QIODevice::ReadOnly))
    {
        setHighscore("0");
        return "0";
    }

    QByteArray dataFromFile = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(dataFromFile);
    if (doc.isNull() || !doc.isObject())
    {
        setHighscore("0");
        return "0";
    }

    QJsonObject obj = doc.object();
    QJsonArray arr = obj["items"].toArray();

    for (const QJsonValue &value : arr)
    {
        QJsonObject parameter = value.toObject();
        highscore = parameter["highscore"].toString();
    }
    return highscore;
}

void MainWindow::setHighscore(QString highscore)
{
    QFile file(JSON_NAME);
    if (file.exists() && !file.remove())
    {
        qDebug() << "Failed to remove existing file:" << file.errorString();
        return;
    }
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return;
    }

    QJsonArray arr;
    arr.append(QJsonObject({
        {"highscore", highscore}
    }));
    QJsonObject obj{{"items", arr}};
    QJsonDocument doc;
    doc.setObject(obj);
    QString highscoreToSet = doc.toJson(QJsonDocument::Indented);

    qint64 bytesWritten = file.write(highscoreToSet.toUtf8());
    if (bytesWritten == -1)
    {
        qDebug() << "Failed to write JSON data to file:" << file.errorString();
        return;
    }
    file.close();
}
