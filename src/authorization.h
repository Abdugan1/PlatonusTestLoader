#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>

namespace Ui {
class Authorization;
}

class NetworkAccessManager;
class Model;

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
    void onCompleterHighlighted(const QModelIndex& index);
private:
    void logIn(const QString& username, const QString& password);
    void setToken(const QString& replyContent);

    void modelFromFile(const QString &fileName);

    Ui::Authorization *ui;
    NetworkAccessManager* networkCtrl_;
    Model *model_;
};

#endif // AUTHORIZATION_H
