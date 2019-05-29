#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void doStartCalcObj(int loops);

private slots:
    void on_run_clicked();

    void on_runThreaded_clicked();

private:
    Ui::MainWindow *ui;


    void startCalc(bool onThread);
};

#endif // MAINWINDOW_H
