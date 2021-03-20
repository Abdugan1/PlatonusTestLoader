#include "networkaccessmanager.h"

#include <QEventLoop>

NetworkAccessManager::NetworkAccessManager(QObject* parent)
    : QNetworkAccessManager(parent)
{

}

void NetworkAccessManager::sendGet(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("token", token_.toUtf8());

    QNetworkReply* reply = QNetworkAccessManager::get(request);
    waitUntillFinished(reply);
    setErrorStatus(reply);
    content_ = reply->readAll();
    reply->deleteLater();
}

void NetworkAccessManager::sendPost(const QUrl& url, const QString& postDataStr)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!token_.isEmpty())
        request.setRawHeader("token", token_.toUtf8());
    QByteArray data(postDataStr.toStdString().c_str());

    QNetworkReply* reply = QNetworkAccessManager::post(request, data);
    waitUntillFinished(reply);
    setErrorStatus(reply);
    content_ = reply->readAll();
    reply->deleteLater();
}

void NetworkAccessManager::sendPost(const QNetworkRequest& request, const QString& postDataStr)
{
    QByteArray data(postDataStr.toStdString().c_str());
    QNetworkReply* reply = QNetworkAccessManager::post(request, data);
    waitUntillFinished(reply);
    setErrorStatus(reply);
    content_ = reply->readAll();
    reply->deleteLater();
}

const ErrorStatus& NetworkAccessManager::errorStatus() const
{
    return errorStatus_;
}

const QString& NetworkAccessManager::content() const
{
    return content_;
}

const QString& NetworkAccessManager::token() const
{
    return token_;
}

void NetworkAccessManager::setToken(const QString& tok)
{
    token_ = tok;
}

void NetworkAccessManager::waitUntillFinished(const QNetworkReply* reply)
{
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void NetworkAccessManager::setErrorStatus(const QNetworkReply* reply)
{
    errorStatus_ = reply->error();
}
