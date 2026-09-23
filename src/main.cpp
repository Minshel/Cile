#include "main-window.hpp"

#include <QApplication>
#include <QByteArray>

#include "qicon.h"
#include "qwidget.h"

int main(int argc, char **argv) {

    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":new_logo.png"));

    QWidget* window = MainWindow::qInit();
    MainWindow::vWidgets(window); // not niggers

    return app.exec();
}
