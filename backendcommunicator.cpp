#include "backendcommunicator.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

BackendCommunicator::BackendCommunicator(QObject *parent)
    : QObject(parent),
    networkManager(new QNetworkAccessManager(this)),
    cookieJar(new QNetworkCookieJar(this))
{
    networkManager->setCookieJar(cookieJar);
}

void BackendCommunicator::login(const QString &username, const QString &password)
{
    QUrl url("http://127.0.0.1:5000/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &BackendCommunicator::onLoginFinished);
}

void BackendCommunicator::fetchSongs()
{
    QUrl url("http://127.0.0.1:5000/songs");
    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &BackendCommunicator::onSongsFetched);
}

void BackendCommunicator::registerUser(const QString &username, const QString &password)
{
    QUrl url("http://127.0.0.1:5000/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &BackendCommunicator::onRegisterFinished);
}

void BackendCommunicator::requestSongUrl(int songId)
{
    QUrl url(QString("http://127.0.0.1:5000/play/%1").arg(songId));
    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &BackendCommunicator::onSongUrlFetched);
}

QList<QNetworkCookie> BackendCommunicator::getCookies()
{
    return cookieJar->cookiesForUrl(QUrl("http://127.0.0.1:5000"));
}

void BackendCommunicator::onSongUrlFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Request failed:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QUrl songUrl = reply->url();

    if (!songUrl.isValid()) {
        qWarning() << "Invalid URL returned.";
        reply->deleteLater();
        return;
    }

    qInfo() << "Song url: " << songUrl.toString();

    emit songFetched(songUrl.toString());

    reply->deleteLater();
}

void BackendCommunicator::onSongsFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Request failed:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    // Read and parse the response
    QByteArray response = reply->readAll();
    qDebug() << "Response:" << response;

    QJsonDocument doc = QJsonDocument::fromJson(response);

    if (doc.isArray()) {
        QJsonArray songArray = doc.array();

        emit songsFetched(songArray);
    } else {
        qWarning() << "Invalid response format, expected JSON array.";
    }

    reply->deleteLater();
}

void BackendCommunicator::onLoginFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Login failed:" << reply->errorString();
        emit loginFinished(false);
        return;
    }

    QList<QNetworkCookie> cookies = getCookies();
    for (const QNetworkCookie &cookie : cookies) {
        qDebug() << "Received cookie:" << cookie.name() << cookie.value();
    }

    fetchSongs();

    emit loginFinished(true);
    reply->deleteLater();
}

void BackendCommunicator::onRegisterFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Registration failed:" << reply->errorString();
        emit registrationFinished(false);
        return;
    }

    QByteArray response = reply->readAll();
    qDebug() << "Registration response:" << response;

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isObject()) {
        QJsonObject jsonResponse = doc.object();

        if (jsonResponse["message"].toString() == "User registered successfully") {
            emit registrationFinished(true);
        } else {
            emit registrationFinished(false);
        }
    }

    reply->deleteLater();
}
