#include "testcombiner.h"
#include "ui_testcombiner.h"
#include "questiondata.h"
#include "internal.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

TestCombiner::TestCombiner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestCombiner)
{
    ui->setupUi(this);
    setFixedSize(this->size());
}

TestCombiner::~TestCombiner()
{
    delete ui;
}


void TestCombiner::on_openFilesButton_clicked()
{
    filePaths_ = QFileDialog::getOpenFileNames(this);

    if (!ui->fileNamesLayout->isEmpty())
        deleteAllFileNames();

    for (const auto& filePath : filePaths_) {
        QString fileName = filePath.mid(filePath.lastIndexOf("/") + 1);
        QLabel* fileNameLabel = new QLabel(fileName);
        ui->fileNamesLayout->insertWidget(0, fileNameLabel);
    }
}

void TestCombiner::deleteAllFileNames()
{
    QLayoutItem *child;
    while ((ui->fileNamesLayout->count() != 1)
           && (child = ui->fileNamesLayout->takeAt(0)) != 0)
    {
        child->widget()->setParent(nullptr);
        delete child;
    }
}

QString TestCombiner::getFilesContent()
{
    QString content;
    for (const auto& filePath : filePaths_) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Warning"), file.errorString());
            return QString();
        }
        QTextStream in(&file);
        content += in.readAll();
    }
    return content;
}

QList<QuestionData> TestCombiner::getQuestionDatas(const QStringList &dataBloks)
{
    QList<QuestionData> questionDatas;
    for (const auto& block : dataBloks) {
        QStringList blocksInfo = Internal::getAllMatches(block.trimmed()
                                                   , QRegularExpression("<p>(.*?)<\\/p>"));
        QuestionData questionData;

        for (const auto& info : blocksInfo) {
            if (info.contains("#question#"))
                questionData.text = info;
            else
                questionData.variants.append(info);
        }
        QString variant = questionData.variants.first();
        questionData.correctAnswered = variant.contains("<font color=\"green\">");

        questionDatas.append(questionData);
    }
    return questionDatas;
}

void TestCombiner::on_combineButton_clicked()
{
    QString content = getFilesContent();
    static const QRegularExpression blockRegex("<!--{-->((?s).*?)<!--}-->");
    QStringList dataBlocks = Internal::getAllMatches(content, blockRegex);

    QList<QuestionData> questionDatas = getQuestionDatas(dataBlocks);

//    emit dataIsReady(this, "", questionDatas);

    int i = 0;
    for (const auto & questionData : questionDatas) {
        qDebug() << ++i;
        qDebug() << "text: " << questionData.text;
        qDebug() << "vars: " << questionData.variants;
        qDebug() << "================================";
    }
}
