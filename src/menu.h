#ifndef MENU_H
#define MENU_H

#include "shape.h"

#include <QGraphicsScene>
#include <QWidget>

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
    void onStartButtonClicked();

public slots:
    void setFocusToStartButton();
    void setNextShape(Shape *shape);
    void setScore(QString score);
    void setHighscore(QString highscore);

private:
    Ui::Menu *ui;

    QGraphicsScene *previewScene;

signals:
    void start();
};

#endif // MENU_H
