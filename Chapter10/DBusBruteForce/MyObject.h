#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <QDebug>

class MyObject : public QObject
{
    Q_OBJECT
public:
    explicit MyObject(QObject *parent = nullptr);

public slots:
    void doIt();
    void pingReceived(QString msg);
};

#endif // MYOBJECT_H
