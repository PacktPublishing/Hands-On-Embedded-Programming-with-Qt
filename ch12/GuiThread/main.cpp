#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set the message format for logging
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} | [%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] | %{function} [%{file}(%{line})] | %{message}");

    MainWindow w;
    w.show();

    return a.exec();
}
