#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CalcObject.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QTime>
#include <QThread>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setRange(0, 1000);
    ui->progressBar->setValue(0);

    ui->loopCount->setValue(2000);

    this->setWindowTitle("QThread Example");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCalc(bool onThread)
{
    qDebug() << "Starting Calc from thread: " << this->thread()->currentThreadId();

    auto cobj = new CalcObject();
    // move to new thread is asked.
    if (onThread)
    {
        cobj->moveToThread(new QThread());
        cobj->thread()->start();
    }


    auto etimer = new QElapsedTimer;
    auto utimer = new QTimer(this);

    ui->progressBar->setValue(0);

    connect(cobj, &CalcObject::CalcProgressUpdate, this, [this] (double prog, int last) {
        ui->progressBar->setValue((int) (prog * 1000.0));
    }, Qt::AutoConnection);

    connect(cobj, &CalcObject::Done, this, [cobj, etimer, utimer] {
        cobj->deleteLater();
        delete etimer;
        utimer->deleteLater();
    }, Qt::AutoConnection);

    connect(utimer, &QTimer::timeout, this, [this, etimer] () {
        auto msec = etimer->elapsed();
        ui->clock->setTime(QTime::fromMSecsSinceStartOfDay(msec));
    }, Qt::AutoConnection);

    etimer->start();
    utimer->setSingleShot(false);
    utimer->start(1000);


    connect(this, &MainWindow::doStartCalcObj, cobj, &CalcObject::DoCalcs, Qt::AutoConnection);
    emit doStartCalcObj(ui->loopCount->value());
//    cobj->DoCalcs(ui->loopCount->value());

    QMessageBox::information(this, "Calculations", "Calculations started");
}

void MainWindow::on_run_clicked()
{
    startCalc(false);
}

void MainWindow::on_runThreaded_clicked()
{
    startCalc(true);
}
