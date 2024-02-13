#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(760, 440);

    int x1 = (1366 - this->width()) / 2;
    int y1 = (768 - this->height()) / 2;

    this->move(x1, y1);
    this->setWindowTitle("Arara Code");

    int y = (this->height() - ui->textBrowser->height()) / 2;
    int x = (this->width() - ui->textBrowser->width()) / 2;

    ui->textBrowser->move(x, y);
}

MainWindow::~MainWindow()
{
    delete ui;
}
