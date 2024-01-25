/* #include "mainwindow.h" */
#include "codeeditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*MainWindow w;
    w.show();*/

    CodeEditor codeEditor;
    codeEditor.show();
    return a.exec();
}
