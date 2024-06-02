#include "mainwindow.h"
#include <cstring>
#include <string>
#include <iostream>
#include "utilities.h"
#include <QApplication>
using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
