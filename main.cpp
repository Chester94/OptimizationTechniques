#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*Factor t("123/1234");

    qDebug() << t.toString() << (t^-1).toString();*/

    return a.exec();
}
