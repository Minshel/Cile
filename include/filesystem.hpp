#pragma once

#include "qpushbutton.h"
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "editor.hpp"
#include "qdialog.h"
#include <QPushButton>
#include "qobject.h"
#include "qwidget.h"
#include "QStringList"
#include "file.hpp"
#include <filesystem>

namespace MainWindow {
    extern File *current_file;
}

namespace FS {
    inline QWidget *filesystem = nullptr;

    void vInit(Editor* editor) {
        if (!filesystem)
            filesystem = new QWidget;
        filesystem->setParent(editor->parentWidget());
        filesystem->resize(300, editor->parentWidget()->height());
        filesystem->setStyleSheet("background: #141414;");

        filesystem->show();
    }

    void vLoadFile(std::filesystem::path path) {
        MainWindow::current_file->filepath = path;
    }

    void vLoadPath(std::filesystem::path path) {
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
            return;
        }

        for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (!std::filesystem::is_regular_file(file.path()))
                continue;

            QString filename = QString::fromStdString(
                file.path().filename().string()
            );

            QPushButton *file_button = new QPushButton(filesystem);
            file_button->resize(300, 35);
            file_button->setText(filename);
            file_button->setIcon(QIcon(":new_logo.png"));

            file_button->setStyleSheet(R"(
                QPushButton {
                    border: none;
                    border-radius: 0px;
                    background: transparent;
                    text-align: left;
                    padding-left: 10px;
                }

                QPushButton:hover {
                    background: #202020;
                }
            )");

            file_button->show();

            const auto file_path = file.path();

            QObject::connect(file_button, &QPushButton::pressed, filesystem, [file_path]() {
                    vLoadFile(file_path);
                }
            );
        }
    }
};

#endif
