#include <QVariant>
#include <QTime>
#include <QDebug>

int main(int argc, char *argv[])
{
    QVariant    v;
    v.setValue(10);

    int y = 41 * v.toInt();         // get the value of v as an int

    qDebug() << v.toString() << " * 41 = " << y;
    qDebug() << "v as a bool = " << v.toBool();
    qDebug() << "v as a list = " << v.toList();


    qDebug() << "--------------------";

    QList<QVariant> l;
    l << v;                         // append v to the list
    l << QVariant(1);               // append the int 1 to list
    l << QVariant(2.3);             // append the double 2.3
    l << QVariant("Hello World!");  // append "Hello World!"

    qDebug() << "The List: " << l;
}
