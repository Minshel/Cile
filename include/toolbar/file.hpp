#pragma once

#include "../main-window.hpp"
#include "../filesystem.hpp"
#include <QFileDialog>
#include "editor.hpp"
#include "../file.hpp"
#include <QObject>
#include <QMenu>
#include <QToolButton>
#include <QToolBar>
#include <QAction>
#include <QMainWindow>
#include <filesystem>
#include <fstream>

namespace Toolbar::File {
    QToolBar* qtbInit(QMainWindow *app, Editor *editor) {
        QToolBar *toolbar = app->addToolBar("File");
        toolbar->setFixedHeight(30);
        toolbar->setMovable(false);

        auto *menu = new QMenu("File", toolbar);




        auto *qaSave = new QAction("Save to file", toolbar);
        QObject::connect(qaSave, &QAction::triggered, app, [editor]() {
            std::ofstream out(MainWindow::current_file->filepath);
            out << editor->toPlainText().toStdString();
        });
        menu->addAction(qaSave);

        auto *qaOpenFile = new QAction("Open File", toolbar);
        QObject::connect(qaOpenFile, &QAction::triggered, app, [editor]() {
            QString file = QFileDialog::getOpenFileName(
                nullptr,
                "Load",
                "",
                "C++ files (*.cpp *.hpp *.cppm *.cc *.cxx);;"
                "C files (*.c *.h *.cc *.cxx);;"
                "Text/Config files (*.ini *.cfg *.toml *.json *.jsonc *.lua *.nix *.log *.conf, *.config, *.yaml, *.yml, *.backup);;"
                "All files (*.*)"
            );

            FS::vLoadFile(std::filesystem::path(file.toStdString()));
        });
        menu->addAction(qaOpenFile);

        auto *qaOpenDir = new QAction("Open Folder", toolbar);
        QObject::connect(qaOpenDir, &QAction::triggered, app, [editor]() {
            QString dir = QFileDialog::getExistingDirectory(
                nullptr,
                "Select",
                QDir::homePath()
            );

            if (!dir.isEmpty()) {
                FS::vLoadPath(std::filesystem::path(dir.toStdString()));
            }
        });
        menu->addAction(qaOpenDir);




        auto *button = new QToolButton(toolbar);
        button->setText("File");
        button->setMenu(menu);
        button->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(button);

        return toolbar;
    }
};
