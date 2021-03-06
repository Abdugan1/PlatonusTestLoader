#include "authorization.h"
#include "ui_authorization.h"
#include "networkaccessmanager.h"
#include "model.h"
#include "internal.h"

#include <QMessageBox>
#include <QCompleter>
#include <QDebug>

Authorization::Authorization(NetworkAccessManager* networkCtrl, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Authorization)
    , networkCtrl_(networkCtrl)
    , model_(new Model(this))
    , completer_(new QCompleter(this))
{
    ui->setupUi(this);

    modelFromFile("logins.txt");

    completer_->setModel(model_);
    completer_->setCaseSensitivity(Qt::CaseInsensitive);

    connect(completer_,  SIGNAL(highlighted(const QModelIndex&)),
            this,       SLOT(onCompleterHighlighted(const QModelIndex&))
            );

    connect(this, SIGNAL(success()),
            this, SLOT(onSuccess())
            );

    ui->usernameEdit->setCompleter(completer_);

    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint
                   | Qt::WindowCloseButtonHint);
    setFixedSize(this->size());
}

Authorization::~Authorization()
{
    delete ui;
}

const QCompleter *Authorization::getCompleter() const
{
    return completer_;
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

void Authorization::onCompleterHighlighted(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    int row = index.row();
    QString password = index.model()->index(row, 1).data().toString();

    ui->passwordEdit->setText(password);
}

void Authorization::onSuccess()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    Model::Data data({username, password});
    if (!model_->contains(data)) {
        int row = model_->rowCount() - 1;
        row = row < 0 ? 0 : row;
        model_->insertRow(row);
        QModelIndex index = model_->index(row, 0);
        model_->setData(index, QVariant::fromValue(data));

        addAuthDataToFile("logins.txt", data);
    }
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

void Authorization::modelFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;
    Model::Data data;
    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        QString content = in.readLine().simplified();
        if (content.isEmpty())
            continue;
        qDebug() << content;
        model_->insertRow(0);
        QModelIndex index = model_->index(0, 0);
        data.username = content.split(" ").first();
        data.password = content.split(" ").at(1);

        model_->setData(index, QVariant::fromValue(data));
    }
    file.close();
}

void Authorization::addAuthDataToFile(const QString &fileName, const Model::Data &data)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
        return;
    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << "\n" << data.username + "\t\t\t\t\t\t\t" + data.password;

    file.close();
}
