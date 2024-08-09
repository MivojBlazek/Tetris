#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"

#include "scene.h"

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *event) override;
    Scene *initScene();

    QString score;

    void increaseSpeed();
    void checkHighscore();
    void addScore(QString addToScore);
    QString getHighscore();
    void setHighscore(QString highscore);
};
#endif // MAINWINDOW_H
