#include <QCoreApplication>

#include "MyObject.h"

#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyObject    obj;
    return a.exec();
}
