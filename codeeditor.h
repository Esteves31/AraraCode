#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

#include "highlighter.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void newFile();
    void openFile(const QString &path);
    void saveFile();
    void saveFileAs();

private:
    void setupEditor();
    void setupMenuBar();

    QMenu *fileMenu;
    QMenuBar *menuBar;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QWidget *lineNumberArea;
    Highlighter *highlighter;
    QTextEdit *editor;
    QString *currentFilePath;
};

#endif // CODEEDITOR_H
