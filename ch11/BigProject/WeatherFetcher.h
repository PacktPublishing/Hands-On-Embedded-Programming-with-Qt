#ifndef WEATHERFETCHER_H
#define WEATHERFETCHER_H

#include <QDateTime>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class WeatherFetcher : public QObject
{
    Q_OBJECT
public:
    explicit WeatherFetcher(QObject *parent = nullptr);

    int updateSecs() const;
    void setUpdateSecs(int updateSecs);

    QString location() const;
    void setLocation(const QString &location);

    int minUpdateSecs() const;
    void setMinUpdateSecs(int minUpdateSecs);

    QDateTime lastUpdate() const;

    QString units() const;
    void setUnits(const QString &units);

    QString key() const;
    void setKey(const QString &key);

signals:
    void UpdatedWeather(QString forecast, QString conditions,
                        double temperature, double hi,
                        double low);

public slots:
    void start();               ///< (re)start updates
    void start(int secs);       ///< start updates after setting update secs
    void stop();                ///< stop getting updates
    bool ForceUpdate();         ///< force an update, true if request okay

private slots:
    void handleApiResponse(QNetworkReply *reply);   ///< handler for the response of the weather API

private:
    QTimer      *m_updateTimer;     ///< the timer for fetching the weather
    int         m_updateSecs;       ///< the number of minutes between updates
    int         m_minUpdateSecs;    ///< minimum time between updates (for API)
    QDateTime   m_lastUpdate;       ///< the date/time of last update
    QString     m_location;         ///< the location the weather is for
    QString     m_units;            ///< the temperature units
    QString     m_key;              ///< the access key for the weather

    QNetworkAccessManager *m_netMan;    ///< the network manager we use

    // ** Methods **
    void doUpdate();                ///< fetch the update

};

#endif // WEATHERFETCHER_H
