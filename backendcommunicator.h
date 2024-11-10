#ifndef BACKENDCOMMUNICATOR_H
#define BACKENDCOMMUNICATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkCookie>
#include <QUrl>

class BackendCommunicator : public QObject
{
    Q_OBJECT

public:
    explicit BackendCommunicator(QObject *parent = nullptr);

    void login(const QString &username, const QString &password);
    void fetchSongs();
    void registerUser(const QString &username, const QString &password);
    void requestSongUrl(int songId);

signals:
    void loginFinished(bool success);
    void songsFetched(const QJsonArray &songs);
    void registrationFinished(bool success);
    void songFetched(const QString &url);

private:
    QNetworkAccessManager *networkManager;
    QNetworkCookieJar *cookieJar;

    void onSongUrlFetched();
    void onSongsFetched();
    void onLoginFinished();
    void onRegisterFinished();
    QList<QNetworkCookie> getCookies();
};

#endif // BACKENDCOMMUNICATOR_H
