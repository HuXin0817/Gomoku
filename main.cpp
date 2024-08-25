#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto *mainWindow = new MainWindow();
    mainWindow->show();
    return QApplication::exec();
}
