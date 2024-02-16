#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(760, 440);
    this->setWindowTitle("Arara Code");

    int y = (this->height() - ui->textBrowser->height()) / 2;
    int x = (this->width() - ui->textBrowser->width()) / 2;

    ui->textBrowser->move(x, y);

    connect(newAction,  &QShortcut::activated, this, &MainWindow::handleNewShortcut);
    connect(openAction, &QShortcut::activated, this, &MainWindow::handleOpenShortcut);
    connect(exitAction, &QShortcut::activated, this, &MainWindow::handleExitShortcut);
}

void MainWindow::handleNewShortcut()
{
    this->close();
}

void MainWindow::handleOpenShortcut()
{
    codeEditor = new CodeEditor();
    codeEditor->resize(940, 580);
    codeEditor->show();
}

void MainWindow::handleExitShortcut()
{
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
