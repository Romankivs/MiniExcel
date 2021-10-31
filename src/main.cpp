#include "window.h"
#include <QApplication>
#include <QTableView>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("MiniExcel");
    app.setWindowIcon(QIcon(":/icons/WinIcon.png"));

    Window mainwin;
    mainwin.show();

    return app.exec();
}
