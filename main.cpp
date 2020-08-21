#include "firstwindow.h"
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/img/s3.png"));

    FirstWindow fw;
    fw.show();

    return a.exec();
}
