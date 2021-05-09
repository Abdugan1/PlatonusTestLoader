#ifndef PLATONUSTESTLOADER_H
#define PLATONUSTESTLOADER_H

#include "testbutton.h"

#include <QMainWindow>

namespace Ui {
class PlatonusTestLoader;
}

class NetworkAccessManager;
struct QuestionData;

typedef TestButton::TestData TestData;

class PlatonusTestLoader : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlatonusTestLoader(NetworkAccessManager* networkCtrl, QWidget *parent = nullptr);
    ~PlatonusTestLoader();

    void downloadTest(const TestData& testData);
    void logOut();
signals:
    void logOuted();
    void dataIsReady(QWidget* parent, const QString& fileName
                     , const QList<QuestionData>& questionDataList);
    void downloadAll(const TestData& testData);
public slots:
    void showTestButtons();
    QList<TestData> getTestsData();
private slots:
    void on_logOutButton_clicked();
    void on_startDateEdit_userDateChanged(const QDate &date);
    void on_finishDateEdit_userDateChanged(const QDate &date);
    void onTestButtonClicked();

private:
    void sendAppealsRequest();
    void setTestsButton(const QList<TestData>& testDataList);
    TestButton* createButton(const TestData& testData);
    void deleteAllTestsButton();
    QString getUserLogin();
    QList<QuestionData> getQuestionsData(const QByteArray &content);
    void highlightIncorrect(QList<QuestionData>& questionDataList, const TestData& testData);

    Ui::PlatonusTestLoader *ui;
    NetworkAccessManager* networkCtrl_;
};

#endif // PLATONUSTESTLOADER_H
