#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

typedef  QNetworkReply::NetworkError ErrorStatus;

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkAccessManager(QObject* parent = nullptr);

    void sendGet(const QUrl& url);
    void sendPost(const QUrl& url, const QString& postDataStr);
    void sendPost(const QNetworkRequest& request, const QString& postDataStr);

    const ErrorStatus& errorStatus() const;
    const QString& content() const;

    const QString& token() const;
    void setToken(const QString& tok);

private:
    void waitUntillFinished(const QNetworkReply* reply);
    void setErrorStatus(const QNetworkReply* reply);

    QString token_;
    QString content_;
    ErrorStatus errorStatus_;
};

#endif // NETWORKACCESSMANAGER_H
