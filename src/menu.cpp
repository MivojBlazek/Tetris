#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    connect(ui->StartButton, &QPushButton::clicked, this, &Menu::onStartButtonClicked);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::onStartButtonClicked()
{
    emit start();
}
