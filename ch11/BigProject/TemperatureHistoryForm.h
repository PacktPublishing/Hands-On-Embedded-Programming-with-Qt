#ifndef TEMPERATUREHISTORYFORM_H
#define TEMPERATUREHISTORYFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QDateTime>
#include <QPointF>

class TemperatureStorage;

namespace Ui {
class TemperatureHistoryForm;
}

class TemperatureHistoryForm : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QPointF lastReading READ lastReading WRITE setLastReading NOTIFY newReading)

public:
    explicit TemperatureHistoryForm(QWidget *parent = nullptr);
    ~TemperatureHistoryForm();

    QPointF lastReading() const;

    void setLastReading(const QPointF &lastReading);


    TemperatureStorage *temperatureStorage() const;
    void setTemperatureStorage(TemperatureStorage *temperatureStorage);

signals:
    void newReading();

public slots:
    void temperatureUpdate(QDateTime timestamp, float temperature);

private:
    Ui::TemperatureHistoryForm *ui;

    QStandardItemModel      m_tempRecord;   //< a record of time & temperature
    QSortFilterProxyModel   m_tempProxy;    //< a way to sort/filter m_tempRecord

    QPointF                 m_lastReading;

    TemperatureStorage      *m_temperatureStorage;
};

#endif // TEMPERATUREHISTORYFORM_H
