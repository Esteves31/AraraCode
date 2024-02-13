#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>

#include "codeeditor.h"
#include "highlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QShortcut *newAction = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_N), this);
    QShortcut *openAction = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this);
    QShortcut *exitAction = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_E), this);

private slots:
    void handleNewShortcut();
    void handleOpenShortcut();
    void handleExitShortcut();

private:
    Highlighter *highlighter;
    Ui::MainWindow *ui;
    CodeEditor *codeEditor;
};
#endif // MAINWINDOW_H
