#include "authorization.h"
#include "ui_authorization.h"
#include "networkaccessmanager.h"
#include "internal.h"

#include <QMessageBox>
#include <QDebug>

Authorization::Authorization(NetworkAccessManager* networkCtrl, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Authorization)
    , networkCtrl_(networkCtrl)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint
                   | Qt::WindowCloseButtonHint);
    setFixedSize(this->size());
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::on_logInButton_clicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {

        if (username.isEmpty())
            ui->usernameEdit->setStyleSheet("border: 1px solid red");;
        if (password.isEmpty())
            ui->passwordEdit->setStyleSheet("border: 1px solid red");;

        return;
    }
    logIn(username, password);
}

void Authorization::logIn(const QString& username, const QString& password)
{
    static const QUrl logInUrl(QStringLiteral("https://edu2.aues.kz/rest/api/login"));
    QString data = "{\"login\":\"" + username + "\",\"iin\":null,\"icNumber\":null,"
                        "\"password\":\"" + password + "\"}";

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    networkCtrl_->sendPost(logInUrl, data);
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    if (networkCtrl_->errorStatus() != ErrorStatus::NoError) {
        QMessageBox::warning(this, tr("Authorization failed"), tr("Invalid password or username"));
        return;
    }

    QString content = networkCtrl_->content();
    setToken(content);
    qDebug() << "class Authorization\ntoken: " << networkCtrl_->token().toUtf8();
    emit success();
}

void Authorization::setToken(const QString& replyContent)
{
    static const QRegularExpression tokenReg("\"auth_token\":\"(.*?)\"");
    QString token = Internal::getAllMatches(replyContent, tokenReg).first();
    networkCtrl_->setToken(token);
}
