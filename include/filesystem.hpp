#pragma once

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "qpushbutton.h"
#include "editor.hpp"
#include "qdialog.h"
#include <QPushButton>
#include <QScrollArea>
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
    inline QScrollArea* scroll = nullptr;
    inline std::filesystem::path old_fspath;

    void vInit(Editor* editor) {
        if (!scroll) {
            scroll = new QScrollArea(editor->parentWidget());
            scroll->resize(300, editor->parentWidget()->height());
            scroll->setStyleSheet("background: #141414;");
            scroll->show();
        }
        if (!filesystem)
            filesystem = new QWidget;
        filesystem->setStyleSheet("background: #141414;");
        scroll->setWidgetResizable(true);
        scroll->setWidget(filesystem);
    }

    void vClear() {
        if (!filesystem)
            return;
        const auto buttons = filesystem->findChildren<QPushButton *>();
        for (QPushButton *button : buttons)
            delete button;
    }

    void vLoadFile(std::filesystem::path path) {
        MainWindow::current_file->filepath = path;
    }

    void vLoadPath(std::filesystem::path fspath, int startY = 0) {
        if (!std::filesystem::exists(fspath) || !std::filesystem::is_directory(fspath)) {
            return;
        }

        vClear();

        for (const auto& file : std::filesystem::directory_iterator(fspath)) {
            const auto file_path = file.path();

            QString filename = QString::fromStdString(
                file.path().filename().string()
            );

            QPushButton *file_button = new QPushButton(filesystem);
            file_button->resize(300, 35);
            file_button->move(0, startY);
            file_button->setIcon(QIcon(":new_logo.png"));

            startY += 35;

            if (std::filesystem::is_directory(file_path)) {
                file_button->setText("/"+filename+"/");
            }else{
                file_button->setText(filename);
            }

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

            QObject::connect(file_button, &QPushButton::pressed, filesystem, [file_path, fspath]() {
                if (std::filesystem::is_directory(file_path)) {
                    old_fspath = fspath;
                    vLoadPath(file_path, 35);

                    QPushButton *back_button = new QPushButton(filesystem);
                    back_button->resize(300, 35);
                    back_button->move(0, 0);
                    back_button->setIcon(QIcon(":new_logo.png"));

                    back_button->setText("<- Back");

                    back_button->setStyleSheet(R"(
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

                    back_button->show();

                    QObject::connect(back_button, &QPushButton::pressed, filesystem, [fspath]() {
                        vLoadPath(old_fspath, 0);
                    });
                }else{
                    vLoadFile(file_path);
                }
            });
        }
    }
};

#endif
