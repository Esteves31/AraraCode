#include "codeeditor.h"
#include "linenumberarea.h"

#include "qpainter.h"
#include "QTextBlock"

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    setupEditor();
    setupMenuBar();
    setupSideBar();

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);
    connect(this, &CodeEditor::fileSaved, this, &CodeEditor::updateFilePath);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    QTextDocument *document = this->document();
    QString currentText = document->toPlainText();

    highlighter = new Highlighter(document);
    highlighter->highlightBlock(currentText);
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), menuBar->height(), 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);

}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top() + menuBar->height(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::blue).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEdit;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document());

    QFile file("codeeditor.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}

void CodeEditor::setupMenuBar()
{
    menuBar = new QMenuBar(this);
    fileMenu = menuBar->addMenu("File");

    newAction = new QAction("New file", this);
    openAction = new QAction("Open file", this);
    saveAction = new QAction("Save file", this);
    saveAsAction = new QAction("Save file as", this);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    connect(newAction, &QAction::triggered, this, &CodeEditor::newFile);
    connect(openAction, &QAction::triggered, [this]() { openFile(QString()); });
    connect(saveAction, &QAction::triggered, this, &CodeEditor::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &CodeEditor::saveFileAs);
}

void CodeEditor::setupSideBar()
{
    sideBar = new QTreeWidget(this);
    sideBar->setColumnCount(1);

    currentDir = QDir::currentPath();

    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setText(0, currentDir.dirName());
    sideBar->addTopLevelItem(root);

}

void CodeEditor::newFile()
{
    this->clear();
}

void CodeEditor::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "All files (*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadWrite | QFile::Text))
            this->setPlainText(file.readAll());
    }

    emit fileSaved(fileName);
}

void CodeEditor::saveFile()
{
    QMessageBox::warning(this, "ERROR", filePath);

    if (!filePath.isEmpty()) {
        QFile file(filePath);

        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "ERROR", "Open this file is not possible!");
        } else {
            QTextStream output(&file);
            output << this->toPlainText();
            file.flush();
            file.close();

            emit fileSaved(filePath);
        }
    }
}

void CodeEditor::updateFilePath(const QString &newFilePath)
{
    filePath = newFilePath;
}

void CodeEditor::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save file", "", "All files (*))");

    if (!filePath.isEmpty()) {
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << this->toPlainText();
            file.flush();
            file.close();
        }
        else
            QMessageBox::warning(this, "ERROR", "Sorry, save this file is not possible!");
    }

    emit fileSaved(filePath);
}









