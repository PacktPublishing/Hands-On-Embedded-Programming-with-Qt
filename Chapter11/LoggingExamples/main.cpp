#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{

    QCoreApplication    a(argc, argv);

    a.setOrganizationName("Packt");
    a.setApplicationName("LoggingExamples");

    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} | [%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] | %{function} [%{file}(%{line})] | %{message}");

    qDebug()    << "This is a debug message";
    qWarning()  << "This is a warning message";
    qInfo()     << "This is an informational message";
    qCritical() << "This is a critical message";

    qFatal("This is a fatal message. Good Bye!");

    qDebug()    << "We never get here.";
}
