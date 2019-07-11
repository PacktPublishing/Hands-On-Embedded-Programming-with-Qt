#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QElapsedTimer;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void RunExample();
    void MapExample();
    void MappedExample();
    void MapReduceExample();

private slots:
    void on_runThreaded_clicked();
    void on_map_clicked();
    void on_mapReduce_clicked();

    void on_mapped_clicked();

private:
    Ui::MainWindow *ui;
    void updateTime(const QElapsedTimer &etimer);
};


#endif // MAINWINDOW_H
