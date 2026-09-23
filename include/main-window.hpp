#pragma once

#include "filesystem.hpp"
#include <filesystem>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow.h"
#include "qtoolbar.h"
#include <string>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QVBoxLayout>
#include <QTimer>
#include "editor.hpp"
#include "file.hpp"
#include "toolbar/file.hpp"

namespace MainWindow {

    extern std::filesystem::path *current_path;
    inline File *current_file = nullptr;
    std::string old_filepath = "";

    inline QWidget *qInit() {
        QMainWindow *app = new QMainWindow;
        app->resize(1920, 1080);
        app->setWindowTitle("Cile");

        QWidget *widget = new QWidget;
        widget->resize(1920, 1080);
        app->setCentralWidget(widget);
        app->show();

        current_file = new File;
        current_file->filepath = "";

        return widget;
    }

    inline void vWidgets(QWidget *window) {
        auto *editor = new Editor(window, QFont("Arial", 19));
        editor->show();

        FS::vInit(editor);

        if (auto *app = qobject_cast<QMainWindow *>(window->parentWidget()))
            Toolbar::File::qtbInit(app, editor);

        auto *timer = new QTimer(window);
        QObject::connect(timer, &QTimer::timeout, window, [editor]() {
            if (current_file->filepath != old_filepath) {
                old_filepath = current_file->filepath;

                editor->vSetLines(
                    current_file->qslReadLines(current_file)
                );
            }
        });
        timer->start(100);
    }

}

#endif
