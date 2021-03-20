#include "platonustestloaderapp.h"
#include "networkaccessmanager.h"
#include "questiondata.h"

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
            out << ("<p><font color=\"orange\">#variant# " + QString("didn't answered") + "</font></p>\n").toUtf8();

        for (const auto& variant : questionData.variants) {
            if (questionData.correctAnswered) {
                out << ("<p><font color=\"green\">#variant# " + variant + "</font></p>\n").toUtf8();
            } else {
                out << ("<p><font color=\"red\">#variant# " + variant + "</font></p>\n").toUtf8();
            }
        }
        out << "<!--}-->\n";
    }

    file.close();
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
