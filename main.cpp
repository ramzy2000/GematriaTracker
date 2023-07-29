#include "mainwindow.h"

#include <QApplication>
#include <QStyle>
#include "fileTools.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(readFile(":/styles/styles.css"));
    MainWindow w;
    w.show();
    return a.exec();
}
