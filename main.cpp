#include "mainwindow.h"
#include "codeeditor.h"

#include <QApplication>
#include <QScreen>
#include <QWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    /*CodeEditor codeEditor;
    codeEditor.resize(940, 580);
    codeEditor.show();*/

    return a.exec();
}
