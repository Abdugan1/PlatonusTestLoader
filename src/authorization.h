#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>

namespace Ui {
class Authorization;
}

class NetworkAccessManager;

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(NetworkAccessManager* networkCtrl, QWidget *parent = nullptr);
    ~Authorization();
signals:
    void success();
private slots:
    void on_logInButton_clicked();

private:
    void logIn(const QString& username, const QString& password);
    void setToken(const QString& replyContent);

    Ui::Authorization *ui;
    NetworkAccessManager* networkCtrl_;
};

#endif // AUTHORIZATION_H
