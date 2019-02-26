#ifndef TEMPERATUREHISTORYFORM_H
#define TEMPERATUREHISTORYFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QDateTime>
#include <QPointF>

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

signals:
    void newReading();

public slots:
    void temperatureUpdate(QDateTime timestamp, float temperature);

private:
    Ui::TemperatureHistoryForm *ui;

    QStandardItemModel      m_tempRecord;   //< a record of time & temperature
    QSortFilterProxyModel   m_tempProxy;    //< a way to sort/filter m_tempRecord

    QPointF                 m_lastReading;
};

#endif // TEMPERATUREHISTORYFORM_H
