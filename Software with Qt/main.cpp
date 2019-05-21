#include "mainwindow.h"
#include <QApplication>
#include <locale.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setlocale(LC_ALL, "C");

    MainWindow w;
    w.showFullScreen();

    return a.exec();
}
