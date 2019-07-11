#include "TemperatureHistoryForm.h"
#include "ui_TemperatureHistoryForm.h"

#include <QStandardItem>
#include <QList>
#include <QVariant>
#include <QQmlEngine>
#include <QQmlContext>

#include <QTimer>

#include "TemperatureStorage.h"

TemperatureHistoryForm::TemperatureHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemperatureHistoryForm)
{
    ui->setupUi(this);

    //** setup the table view **
    // define the headers for the temperature history
    m_tempRecord.setHorizontalHeaderLabels(QStringList({"Date & Time", "Temperature"}));

    // tell the sort filter proxy what model to data from
    //-- m_tempProxy.setSourceModel(&m_tempRecord);

    // tell the table view what model to use
    ui->tableView->setModel(&m_tempProxy);

    ui->splitter->setSizes({100, 100});

    //** setup the QtQuick Chart **
    ui->quickWidget->resize(QSize(640, 480));
    ui->quickWidget->engine()->rootContext()->setContextProperty("temperatureData", this);
    ui->quickWidget->setSource(QUrl("qrc:/TemperatureChart.qml"));
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
}

TemperatureHistoryForm::~TemperatureHistoryForm()
{
    delete ui;
}

void TemperatureHistoryForm::temperatureUpdate(QDateTime timestamp, float temperature)
{
    // ** update the table **
    // create a row with the timestamp and temperature.
    auto time = new QStandardItem;  // a place for the timestamp
    auto temp = new QStandardItem;  // a place for the temperature

    // set the displayable data (Qt::DisplayRole)
    time->setData(timestamp, Qt::DisplayRole);
    temp->setData(temperature, Qt::DisplayRole);

    // create the row
    QList<QStandardItem *> row;
    row << time << temp;

    // add the row to the model
    m_tempRecord.appendRow(row);

    // ** update the m_lastReading **
    double xVal = timestamp.toSecsSinceEpoch();
    setLastReading(QPointF(xVal, temperature));
}

TemperatureStorage *TemperatureHistoryForm::temperatureStorage() const
{
    return m_temperatureStorage;
}

void TemperatureHistoryForm::setTemperatureStorage(TemperatureStorage *temperatureStorage)
{
    m_temperatureStorage = temperatureStorage;
    m_tempProxy.setSourceModel(m_temperatureStorage->temperatureModel());
}

void TemperatureHistoryForm::setLastReading(const QPointF &lastReading)
{
    m_lastReading = lastReading;
    emit newReading();
}

QPointF TemperatureHistoryForm::lastReading() const
{
    return m_lastReading;
}
