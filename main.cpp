#include "mainwindow.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
    std::srand((unsigned int)std::time(0)); // 保证随机数
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
