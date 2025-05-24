#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("C:\\Users\\user\\Documents\\QRGENERATOR\\qr.ico"));

    MainWindow w;
    w.show();

    return a.exec();
}
