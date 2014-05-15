#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*Factor t("1.6");

    qDebug() << t.toString();*/

    return a.exec();
}
