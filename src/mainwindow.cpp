#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "menu.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(750, 750);
    Scene *scene = initScene();

    QGraphicsView *view = ui->graphicsView;
    view->setScene(scene);
    view->setFocusPolicy(Qt::StrongFocus);

    connect(ui->Right_menu, &Menu::start, scene, [scene, view]() {
        scene->startButtonPressed();
        view->setFocus();
    });

    connect(scene, &Scene::nextShapeGenerated, ui->Right_menu, &Menu::setNextShape);
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
