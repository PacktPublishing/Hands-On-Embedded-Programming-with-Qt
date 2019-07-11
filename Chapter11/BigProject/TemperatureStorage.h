#ifndef TEMPERATURESTORAGE_H
#define TEMPERATURESTORAGE_H

#include "TemperatureReading.h"

#include <QList>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

class TemperatureStorage
{
public:
    TemperatureStorage();
    ~TemperatureStorage();

    /// Open the Storage
    bool OpenStorage(const QString &id = "readings");

    /// Close the Storage
    void CloseStorage();

    /// get the storage id
    QString id() const;

    /// Add a Reading
    void AddReading(const TemperatureReading &reading);

    /// Retrieve all readings
    QList<TemperatureReading>   GetAllReadings() const;

    /// Get the last n readings
    QList<TemperatureReading>   GetLastNReadings(int n) const;

    /// Get the last reading
    TemperatureReading  GetLastReading() const;

    /// Clear all readings from storage
    void ClearAllReadings();

    /// Get the number of stored readings
    int NumberOfReadings() const;

    QSqlQueryModel *temperatureModel() const;

private:
    QString         m_id;   ///< the storage id
    QSqlDatabase    m_db;   ///< the database

    QSqlQueryModel  *m_temperatureModel;    ///< the temperature model
};

#endif // TEMPERATURESTORAGE_H
