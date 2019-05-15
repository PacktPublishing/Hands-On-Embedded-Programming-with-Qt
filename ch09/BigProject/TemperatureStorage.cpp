#include "TemperatureStorage.h"

#include <QFile>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

TemperatureStorage::TemperatureStorage()
{
    // nothing here
}

TemperatureStorage::~TemperatureStorage()
{
    // this space for rent
}

bool TemperatureStorage::OpenStorage(const QString &id)
{
    QString dbName = QString("ReadingStore_%1").arg(id);
    QString dbFileName = QString("%1.sqlite").arg(dbName);

    // Create a database interface for our SQLite databased, dbName
    m_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    m_db.setDatabaseName(dbFileName);

    bool result = m_db.open();
    if (result)                     // if the db was opened
    {
        // create the table if it does not exist
        QSqlQuery   query(m_db);    // build query
        result = query.exec("create table if not exists TemperatureHistory ("
                            "  timestamp        datetime primary key,"
                            "  temperature      float"
                            ");"
                            );

        // create a QSqlQueryModel
        m_temperatureModel = new QSqlQueryModel;
        m_temperatureModel->setQuery("select * from TemperatureHistory order by timestamp desc;", m_db);
    }

    if (!result) {
        // print debug message if the database did not open
        qDebug() << "!!! Database Failure:" << m_db.lastError();
    }
    return result;
}

void TemperatureStorage::CloseStorage()
{
    m_db.close();
}

QString TemperatureStorage::id() const
{
    return m_id;
}

void TemperatureStorage::AddReading(const TemperatureReading &reading)
{
    QSqlQuery   query(m_db);
    query.prepare("insert into TemperatureHistory values (:timestamp, :temperature);");
    query.bindValue(":timestamp",   reading.timestamp());
    query.bindValue(":temperature", reading.temperatureC());
    query.exec();

    m_temperatureModel->setQuery("select * from TemperatureHistory order by timestamp desc;", m_db);
}

QList<TemperatureReading> TemperatureStorage::GetAllReadings() const
{
    QList<TemperatureReading>   data;

    // Example using QSqlTableModel
    QSqlTableModel  model(nullptr, m_db);
    model.setTable("TemperatureHistory");
    model.select();                         // get the data

    for (int row = 0; row < model.rowCount(); ++row)
    {
        auto rec = model.record(row);
        data << TemperatureReading(rec.value("timestamp").toDateTime(),
                                   rec.value("temperature").toFloat());
    }
    return data;
}

QList<TemperatureReading> TemperatureStorage::GetLastNReadings(int n) const
{
    QList<TemperatureReading>   data;

    // Retrieve using QSqlQuery
    auto qStr = QString("select * from TemperatureHistory order by timestamp desc limit %1;").arg(n);
    QSqlQuery   query(qStr, m_db);
    query.exec();
    while (query.next())
    {
        data << TemperatureReading(query.value("timestamp").toDateTime(),
                                   query.value("temperature").toFloat());
    }

    return data;
}

TemperatureReading TemperatureStorage::GetLastReading() const
{
    auto data = GetLastNReadings(1);
    return data.value(0, TemperatureReading());
}

void TemperatureStorage::ClearAllReadings()
{
    QSqlQuery   query("delete from TemperatureHistory where 1;", m_db);
    query.exec();

    m_temperatureModel->query().exec();
}

int TemperatureStorage::NumberOfReadings() const
{
    QSqlQuery   query("select count(timestamp) from TemperatureHistory;", m_db);
    query.exec();
    query.next();
    return query.value(1).toInt();
}

QSqlQueryModel *TemperatureStorage::temperatureModel() const
{
    return m_temperatureModel;
}
