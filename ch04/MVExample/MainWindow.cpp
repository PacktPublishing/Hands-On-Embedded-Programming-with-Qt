#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // associate the views with the model
    ui->listView->setModel(&m_model);
    ui->treeView->setModel(&m_model);
    ui->tableView->setModel(&m_model);
    ui->columnView->setModel(&m_model);

    BuildModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BuildModel()
{
    for (int i = 0; i < 10; ++i)
    {
        QList<QStandardItem *> items;
        for (int j = 0; j < 5; ++j)
        {
            auto item = new QStandardItem(QString("(%1, %2)").arg(i+1).arg(j+1));
            items << item;

            // add a sub row if the first item in row
            if (j == 0)
            {
                // Add several sets of sub items
                for (int k = 0; k < 3; ++k)
                {
                    QList<QStandardItem *> sitems;
                    for (char c = 'A'; c < 'A' + 5; ++c)
                    {
                        auto sitem = new QStandardItem(QString("%1.%2%3").arg(item->data(Qt::DisplayRole).toString())
                                                       .arg(c).arg(k));
                        sitems << sitem;

                        // add a sub-sub row if the first item in row
                        if (c == 'A')
                        {
                            QList<QStandardItem *> ssitems;
                            for (char sc = 'a'; sc < 'a' + 5; ++sc)
                            {
                                auto ssitem = new QStandardItem(QString("%1.%2").arg(sitem->data(Qt::DisplayRole).toString())
                                                                .arg(sc));
                                ssitems << ssitem;
                            }
                            sitem->appendRow(ssitems);
                        }

                    }
                    item->appendRow(sitems);
                }
            }
        }
        m_model.appendRow(items);
    }
}
