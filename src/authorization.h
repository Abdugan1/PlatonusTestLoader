#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include "model.h"
#include <QWidget>
#include <QCompleter>

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

    const QCompleter *getCompleter() const;
    void logIn(const QString& username, const QString& password);
signals:
    void success();
private slots:
    void on_logInButton_clicked();
    void onCompleterHighlighted(const QModelIndex& index);
    void onSuccess();
private:
    void setToken(const QString& replyContent);

    void modelFromFile(const QString &fileName);
    void addAuthDataToFile(const QString &fileName, const Model::Data &data);

    Ui::Authorization *ui;
    NetworkAccessManager* networkCtrl_;
    Model *model_;
    QCompleter *completer_;
};

#endif // AUTHORIZATION_H
