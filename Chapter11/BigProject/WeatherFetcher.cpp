#include "WeatherFetcher.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QSettings>

#include <QDebug>

WeatherFetcher::WeatherFetcher(QObject *parent) :
    QObject(parent),
    m_updateSecs(15 * 60),
    m_minUpdateSecs(60)
{
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    m_updateTimer->setInterval(m_updateSecs);

    connect(m_updateTimer, &QTimer::timeout, this, &WeatherFetcher::doUpdate);

    m_netMan = new QNetworkAccessManager(this);
    connect(m_netMan, &QNetworkAccessManager::finished,
            this, &WeatherFetcher::handleApiResponse);
}

int WeatherFetcher::updateSecs() const
{
    return m_updateSecs;
}

void WeatherFetcher::setUpdateSecs(int updateSecs)
{
    if (updateSecs >= m_minUpdateSecs)
    {
        m_updateSecs = updateSecs;
    }
}

QString WeatherFetcher::location() const
{
    return m_location;
}

void WeatherFetcher::setLocation(const QString &location)
{
    m_location = location;
}

int WeatherFetcher::minUpdateSecs() const
{
    return m_minUpdateSecs;
}

void WeatherFetcher::setMinUpdateSecs(int minUpdateSecs)
{
    m_minUpdateSecs = minUpdateSecs;
}

bool WeatherFetcher::ForceUpdate()
{
    auto currentSecs = QDateTime::currentSecsSinceEpoch();
    auto timeSince = currentSecs - m_lastUpdate.toSecsSinceEpoch();
    bool requestOk = timeSince >= minUpdateSecs();
    if (requestOk)
    {
        doUpdate();
    }
    return requestOk;
}

void WeatherFetcher::handleApiResponse(QNetworkReply *reply)
{
    auto data = reply->readAll();
    QJsonDocument   jDoc = QJsonDocument::fromJson(data);

    auto qObj = jDoc.object();

    QJsonObject mainSection = qObj.value("main").toObject();
    double temp         = mainSection.value("temp").toDouble();
    double lowtemp      = mainSection.value("temp_min").toDouble();
    double hightemp     = mainSection.value("temp_max").toDouble();

    auto weather  = qObj["weather"].toArray();
    QStringList weatherPieces;
    for (auto w : weather)
    {
        auto element = w.toObject();
        weatherPieces << element["main"].toString();
    }
    QString weatherDesc = weatherPieces.join(", ");

    QString forecast;   // no data, so nothing to set

    reply->deleteLater();

    // send out the message
    emit UpdatedWeather(forecast, weatherDesc, temp, hightemp, lowtemp);

}

QString WeatherFetcher::key() const
{
    return m_key;
}

void WeatherFetcher::setKey(const QString &key)
{
    m_key = key;
}

QString WeatherFetcher::units() const
{
    return m_units;
}

void WeatherFetcher::setUnits(const QString &units)
{
    if (units.contains("Fahrenheit"))
    {
        m_units = "imperial";
    }
    else if (units.contains("Celsius"))
    {
        m_units = "metric";
    }
    else
    {
        m_units = "xxx";
    }
}

QDateTime WeatherFetcher::lastUpdate() const
{
    return m_lastUpdate;
}

void WeatherFetcher::start()
{
    doUpdate();
}

void WeatherFetcher::start(int secs)
{
    m_updateSecs = secs;
    doUpdate();
}

void WeatherFetcher::stop()
{
    m_updateTimer->stop();
}

void WeatherFetcher::doUpdate()
{
    QString query = QStringLiteral("http://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=%3")
            .arg(m_location, m_key, m_units);

    m_netMan->get(QNetworkRequest(QUrl(query)));

    m_lastUpdate = QDateTime::currentDateTime();
    m_updateTimer->start(m_updateSecs * 1000);
}
