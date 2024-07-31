#ifndef MENU_H
#define MENU_H

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
    void onStopButtonClicked();

private:
    Ui::Menu *ui;

signals:
    void start();
    void stop();
};

#endif // MENU_H
