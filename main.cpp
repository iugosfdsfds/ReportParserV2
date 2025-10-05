#include "mainwindow.h"

#include <QApplication>

#if QT_VERSION_MAJOR >= 6
#include <QStyleHints>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
