#include "Person.h"

#include <QDebug>
#include <QMetaProperty>

int main(int argc, char *argv[])
{
    Person p;
    p.setProperty("name", "John");
    p.setProperty("birthday", QDate(2000, 9, 24));    // September 24, 2000
    p.setProperty("sex", "male");

    p.setProperty("haircolor", "brown");

    qDebug() << "--- Get via Properties ---";
    qDebug() << p.property("name") << " : " << p.property("birthday") << " : " << p.property("sex") << p.property("haircolor");

    qDebug() << "--- Get attributes directly ---";
    qDebug() << p.name() << " : " << p.birthday() << " : " << p.sex();

    qDebug() << "--- Get all properties of p via iteration---";
    qDebug() << "- static -";
    for (int i = 0; i < p.staticMetaObject.propertyCount(); ++i)
    {
        auto prop = p.staticMetaObject.property(i);
        qDebug() << prop.name() << " = " << prop.read(&p);
    }
    qDebug() << "- dynamic -";
    for (auto i : p.dynamicPropertyNames())
    {
        qDebug() << i << " = " << p.property(i);
    }
}
