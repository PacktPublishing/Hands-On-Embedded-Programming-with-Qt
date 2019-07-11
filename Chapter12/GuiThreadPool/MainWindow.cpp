#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CalcObject.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QTime>
#include <QThreadPool>
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
    ui->threadCount->setValue(100);
    ui->poolSize->setValue(20);

    this->setWindowTitle("QThreadPool/QRunnable Example");

    m_threadPool = new  QThreadPool(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCalc()
{
    qDebug() << "Starting Calc from thread: " << this->thread()->currentThreadId();

    m_totalDone = 0;
    m_threadPool->setMaxThreadCount(ui->poolSize->value());

    ui->progressBar->setRange(0, ui->loopCount->value());
    ui->progressBar->setValue(0);

    auto loopsPer = ui->loopCount->value() / ui->threadCount->value();
    auto etimer = new QElapsedTimer;
    auto utimer = new QTimer(this);

    etimer->start();
    utimer->setSingleShot(false);
    utimer->start(1000);

    connect(utimer, &QTimer::timeout, this, [this, etimer, utimer] () {
        auto msec = etimer->elapsed();
        ui->clock->setTime(QTime::fromMSecsSinceStartOfDay(msec));
        auto activeThreads = m_threadPool->activeThreadCount();
        ui->activeThreads->display(activeThreads);
        if (activeThreads == 0)
        {
            delete etimer;
            utimer->deleteLater();
        }
    }, Qt::AutoConnection);


    for (int tc = 0; tc < ui->threadCount->value(); ++tc)
    {
        auto cobj = new CalcObject();

        // assign loopsPer and make sure remainder added to first
        if (tc != 0)
        {
            cobj->setLoopCount(loopsPer);
        }
        else
        {
            cobj->setLoopCount(loopsPer + (ui->loopCount->value() % ui->threadCount->value()));
        }

        connect(cobj, &CalcObject::CalcProgressUpdate, this, [this] (double prog, int last) {
            m_totalDone += last;
            ui->progressBar->setValue(m_totalDone);
        }, Qt::AutoConnection);

        // start the QRunnable Object
        m_threadPool->start(cobj);
    }

    QMessageBox::information(this, "Calculations", "Calculations started");
}

void MainWindow::on_runThreaded_clicked()
{
    startCalc();
}
