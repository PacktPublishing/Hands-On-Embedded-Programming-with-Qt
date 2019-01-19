#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    ui->label->setText(QString("Nice to meet you %1!").arg(name));

    static int ssCount = 0;
    QTimer::singleShot(200, [this]() {this->grab().save(QString("screenshot_%1.png").arg(ssCount));});
}
