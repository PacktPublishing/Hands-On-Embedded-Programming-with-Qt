#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QElapsedTimer>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QVector>
#include <QMessageBox>
#include <algorithm>


int sumToN(int n) {
    int sum = 0;
    for (int i = 1; i < n+1; ++i) {
        sum += i;
    }
}

int sumRandom(int loopCount) {
    int sum = 0;
    for (int i = 0; i < loopCount; ++i)
    {
        for (int j = 0; j < loopCount/2; ++j)
        {
            sum += QRandomGenerator64::system()->generate() % 100000;
        }
    }
    return sum;
}

void sumRandomReplace(int &i) {
    i = sumRandom(i);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    this->setWindowTitle("Concurrent");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime(const QElapsedTimer &etimer)
{
    auto elapsed = etimer.elapsed();
    auto time = QTime::fromMSecsSinceStartOfDay(elapsed);
    ui->clock->setTime(time);
}

void MainWindow::on_runThreaded_clicked()
{
    RunExample();
}


void MainWindow::on_map_clicked()
{
    MapExample();
}

void MainWindow::on_mapReduce_clicked()
{
    MapReduceExample();
}

void MainWindow::on_mapped_clicked()
{
    MappedExample();
}

void MainWindow::RunExample()
{
    // setup
    QElapsedTimer   etimer;
    etimer.start();

    qDebug() << __FUNCTION__ << "-- Started --";
    ui->progressBar->setValue(0);   // display progress
    updateTime(etimer);             // update time

    // run with 3 different values of n
    QFuture<int> f1 = QtConcurrent::run(sumRandom, 1000);
    QFuture<int> f2 = QtConcurrent::run(sumRandom, 6421);
    QFuture<int> f3 = QtConcurrent::run(sumRandom, 522);

    auto fw1 = new QFutureWatcher<int>(this);
    auto fw2 = new QFutureWatcher<int>(this);
    auto fw3 = new QFutureWatcher<int>(this);
    fw1->setFuture(f1);
    fw2->setFuture(f2);
    fw3->setFuture(f3);

    connect(fw1, &QFutureWatcher<int>::finished,
            this, [=] () {
                ui->progressBar->setValue(25);   // display progress
                updateTime(etimer);             // update time
                qDebug() << "fw1 done, Time = " << etimer.elapsed();
                QMessageBox::information(this, "Future Finished", QString("f1 returned %1").arg(fw1->result()));
                fw1->deleteLater();
    });

    connect(fw2, &QFutureWatcher<int>::finished,
            this, [=] () {
                ui->progressBar->setValue(50);   // display progress
                updateTime(etimer);             // update time
                qDebug() << "fw2 done, Time = " << etimer.elapsed();
                QMessageBox::information(this, "Future Finished", QString("f2 returned %1").arg(fw2->result()));
                fw2->deleteLater();
    });

    connect(fw3, &QFutureWatcher<int>::finished,
            this, [=] () {
                ui->progressBar->setValue(75);   // display progress
                updateTime(etimer);             // update time
                qDebug() << "fw3 done, Time = " << etimer.elapsed();
                QMessageBox::information(this, "Future Finished", QString("f3 returned %1").arg(fw3->result()));
                fw3->deleteLater();
    });

    auto sync = new QFutureSynchronizer<int>();
    sync->addFuture(f1);
    sync->addFuture(f2);
    sync->addFuture(f3);

    QtConcurrent::run([sync] () {
        sync->waitForFinished();
        delete sync;
    });

//    ui->progressBar->setValue(100); // display progress
//    updateTime(etimer);             // update time

//    qDebug() << __FUNCTION__ << "Time =" << etimer.elapsed();
//    qDebug() << __FUNCTION__ << f1.results();
//    qDebug() << __FUNCTION__ << f2.results();
//    qDebug() << __FUNCTION__ << f3.results();

//    QMessageBox::information(this, "All Threads Done", "All threads done.");

}

void MainWindow::MapExample()
{
    // setup
    QElapsedTimer   etimer;
    etimer.start();

    qDebug() << __FUNCTION__ << "-- Started --";
    ui->progressBar->setValue(0);   // display progress
    updateTime(etimer);             // update time

    QVector<int>  values({1000, 6421, 522});

    // do calcs
    QFuture<void> future = QtConcurrent::map(values, sumRandomReplace);

    ui->progressBar->setValue(50);  // display progress
    updateTime(etimer);             // update time

    future.waitForFinished();

    ui->progressBar->setValue(100); // display progress
    updateTime(etimer);             // update time

    auto elapsed = etimer.elapsed();
    qDebug() << __FUNCTION__ << "Time =" << elapsed;
    qDebug() << __FUNCTION__ << values;

    QMessageBox::information(this, "Map Done", QString("Time = %1").arg(elapsed));
}

void MainWindow::MappedExample()
{
    // setup
    QElapsedTimer   etimer;
    etimer.start();

    qDebug() << __FUNCTION__ << "-- Started --";
    ui->progressBar->setValue(0);   // display progress
    updateTime(etimer);             // update time

    QList<int>  values({1000, 6421, 522});

    QFuture<int> f = QtConcurrent::mapped(values, sumRandom);

    ui->progressBar->setValue(50);  // display progress
    updateTime(etimer);             // update time

    auto sync = new QFutureSynchronizer<int>();
    sync->addFuture(f);
    sync->waitForFinished();

    ui->progressBar->setValue(100);  // display progress
    updateTime(etimer);             // update time

    auto elapsed = etimer.elapsed();
    qDebug() << __FUNCTION__ << "Time =" << elapsed;
    qDebug() << __FUNCTION__ << f.results();

    QMessageBox::information(this, "Mapped Done", QString("Time = %1").arg(elapsed));
}

void workingAvg(int &avg, int v) {
    static int count = 0;
    static int sum = 0;
    sum += v;
    ++count;
    avg = sum/count;
}

void MainWindow::MapReduceExample()
{
    // setup
    QElapsedTimer   etimer;
    etimer.start();

    qDebug() << __FUNCTION__ << "-- Started --";
    ui->progressBar->setValue(0);   // display progress
    updateTime(etimer);             // update time

    QList<int>  values({1000, 6421, 522});

    QFuture<int> f = QtConcurrent::mappedReduced(values, sumRandom, workingAvg);

    ui->progressBar->setValue(50);  // display progress
    updateTime(etimer);             // update time

    auto result = f.result();

    ui->progressBar->setValue(100);  // display progress
    updateTime(etimer);             // update time

    auto elapsed = etimer.elapsed();
    qDebug() << __FUNCTION__ << "Time =" << elapsed;
    qDebug() << __FUNCTION__ << result;

    QMessageBox::information(this, "Map-Reduce Done",
                             QString("Time = %1, Result = %2").arg(elapsed).arg(result));
}



