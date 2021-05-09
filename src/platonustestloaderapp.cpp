#include "platonustestloaderapp.h"
#include "networkaccessmanager.h"
#include "questiondata.h"
#include "model.h"

#include <QFileDialog>
#include <QMessageBox>

PlatonusTestLoaderApp::PlatonusTestLoaderApp(QObject* parent)
    : QObject(parent)
    , networkCtrl_(new NetworkAccessManager(this))
    , authorization_(networkCtrl_)
    , platonusTestLoader_(networkCtrl_)
{
    connect(&authorization_, SIGNAL(success()), this, SLOT(onSuccessAuthorization()));
    connect(&authorization_, SIGNAL(success()), &platonusTestLoader_, SLOT(showTestButtons()));
    connect(&platonusTestLoader_, SIGNAL(logOuted()), this, SLOT(onLogOut()));

    connect(&platonusTestLoader_, &PlatonusTestLoader::dataIsReady, this, &PlatonusTestLoaderApp::saveFile);
    connect(&testCombiner_, &TestCombiner::dataIsReady, this, &PlatonusTestLoaderApp::saveFile);

    connect(&platonusTestLoader_, &PlatonusTestLoader::downloadAll, this, &PlatonusTestLoaderApp::onDownloadAll);

    authorization_.show();
    testCombiner_.show();
}

void PlatonusTestLoaderApp::saveFile(QWidget *parent, const QString &fileName, const QList<QuestionData> &questionDataList)
{
    QString filePath = QFileDialog::getSaveFileName(parent, tr("Save File"), "./" + fileName + ".html");
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(parent, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);

    int i = 0;
    for (const auto& questionData : questionDataList) {
        out << "<!--{-->\n";
        out << ("<p>" + QString::number(++i) + ") #question# " + questionData.text + "</p>\n").toUtf8();

        if (questionData.variants.isEmpty())
            out << ("<p><font color=\"orange\">#variant# " + QString("didn't answered<!--selected:true-->") + "</font></p>\n").toUtf8();

        // leave only the correct answer
        if (questionData.correctAnswered) {
            for (const auto& variant : qAsConst(questionData.variants)) {
                if (variant.contains("<!--selected:true-->")) {
                    out << ("<p><font color=\"green\">#variant# " + variant + "</font></p>\n").toUtf8();
                    break;
                }
            }
        } else { // highlight answered as red, others as gray
            for (const auto& variant : qAsConst(questionData.variants)) {
                if (variant.contains("<!--selected:true-->")) {
                    out << ("<p><font color=\"red\">#variant# " + variant + "</font></p>\n").toUtf8();
                } else {
                    out << ("<p><font color=\"dimgrey\">#variant# " + variant + "</font></p>\n").toUtf8();
                }
            }
        }
        out << "<!--}-->\n";
    }

    file.close();
}

void PlatonusTestLoaderApp::onDownloadAll(const TestData &testData)
{
    struct UserData
    {
        QString username;
        QString password;
    };
    QList<UserData> userDataList;

    int count = authorization_.getCompleter()->model()->rowCount();
    userDataList.reserve(count);
    for (int i = 0; i < count ; ++i) {
        QModelIndex unameIndex = authorization_.getCompleter()->model()->index(i, 0);
        QModelIndex pwordIndex = authorization_.getCompleter()->model()->index(i, 1);

        QString username = unameIndex.data().toString();
        QString password = pwordIndex.data().toString();

        authorization_.logIn(username, password);
        QList<TestData> testDataList = platonusTestLoader_.getTestsData();
        int size = testDataList.size();
        for (int j = 0; j < size; ++j) {
            TestData tData = testDataList.at(j);
            if ((tData.id == testData.id) && (tData.name == testData.name)) {
                platonusTestLoader_.downloadTest(testData);
            }
        }
        platonusTestLoader_.logOut();
    }

    QList<TestData> testDataList = platonusTestLoader_.getTestsData();
}

void PlatonusTestLoaderApp::onSuccessAuthorization()
{
    authorization_.close();
    platonusTestLoader_.show();
}

void PlatonusTestLoaderApp::onLogOut()
{
    platonusTestLoader_.close();
    authorization_.show();
}
