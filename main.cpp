#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmapClose(":/new/resources/open-hand.svg");
    QPixmap newPixmap = pixmapClose.scaled(QSize(200,200),  Qt::KeepAspectRatio);

    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(newPixmap));
    splash->show();

    MainWindow w;
    w.show();
    splash->finish(&w);
    return a.exec();
}
