#pragma once

#include "qfont.h"
#ifndef EDITOR_H
#define EDITOR_H

#include "QColor"
#include "QFont"
#include <QPlainTextEdit>
#include <QPainter>
#include <QRectF>
#include <QResizeEvent>
#include <QTextBlock>
#include <QStringList>
#include <QWidget>

class LineNumbers;

class Editor : public QPlainTextEdit {
public:
    LineNumbers *lines = nullptr;

    Editor(QWidget *window, QFont font);

    void vSetLines(QStringList lines);

    QTextBlock firstBlock() {
        return firstVisibleBlock();
    }

    QPointF contentOffset() {
        return QPlainTextEdit::contentOffset();
    }

    QRectF blockRect(const QTextBlock &block) {
        return blockBoundingGeometry(block).translated(contentOffset());
    }

protected:
    void resizeEvent(QResizeEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;
};

class LineNumbers : public QWidget {
public:
    Editor *editor;

    LineNumbers(Editor *editor)
        : QWidget(editor),
          editor(editor)
    {
        setGeometry(0, 0, 100, editor->height());
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setPen(QColor("#8F8F8F"));
        painter.setFont(QFont(editor->font().family(), editor->font().pointSize()-3));

        painter.fillRect(event->rect(), QColor("#161616"));

        QTextBlock block = editor->firstBlock();

        while (block.isValid()) {
            QRectF geometry = editor->blockRect(block);

            if (geometry.bottom() < 0) {
                block = block.next();
                continue;
            }
            if (geometry.top() > height())
                break;

            painter.drawText(
                0,
                (geometry.top() + 3.5),
                width() - 5,
                geometry.height(),
                Qt::AlignRight,
                QString::number(block.blockNumber() + 1)
            );

            block = block.next();
        }
    }
};

inline void Editor::vSetLines(QStringList lines) {
    setPlainText(lines.join("\n"));
    if (this->lines)
        this->lines->update();
}

inline void Editor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    if (lines)
        lines->setGeometry(0, 0, 100, height());
}

inline bool Editor::eventFilter(QObject *obj, QEvent *event) {
    if (obj == parentWidget() && event->type() == QEvent::Resize) {
        int width = 1620;
        int avail = parentWidget()->width() - 300;
        if (avail < width)
            width = avail > 0 ? avail : 0;

        setGeometry(300, 0, width, parentWidget()->height());
        if (lines)
            lines->setGeometry(0, 0, 100, height());
    }
    return QPlainTextEdit::eventFilter(obj, event);
}

inline Editor::Editor(QWidget *window, QFont font)
    : QPlainTextEdit(window) {
    setFont(font);
    setTabStopDistance(fontMetrics().horizontalAdvance(' ') * 4);
    move(300, 0);

    QResizeEvent e(window->size(), QSize());
    eventFilter(window, &e);

    setViewportMargins(100, 0, 0, 0);

    setPlainText("");

    lines = new LineNumbers(this);
    lines->show();

    window->installEventFilter(this);

    connect(this, &QPlainTextEdit::updateRequest, this, [this](const QRect &rect, int dy) {
        if (dy)
            lines->scroll(0, dy);
        lines->update(0, rect.y(), lines->width(), rect.height());
    });
}

#endif
