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

TestCombiner::QuestionDatas TestCombiner::getQuestionDatas(const QStringList &dataBloks)
{
    QuestionDatas questionDatas;
    int i = 0;
    for (const auto& block : dataBloks) {
        QStringList blocksInfo = Internal::getAllMatches(block.trimmed()
                                                   , QRegularExpression("<p>(.*?)<\\/p>"));
        QuestionData questionData;

        for (auto& info : blocksInfo) {
            if (info.contains("#question#"))
                questionData.text = info.remove(QRegularExpression("\\d+\\) "));
            else
                questionData.variants.append(info);
        }
        QString variant = questionData.variants.first();
        questionData.correctAnswered = variant.contains("<font color=\"green\">");

        questionDatas.insert(questionData.text, questionData);
        i++;
    }

    for (const auto& questionData : questionDatas) {
        qDebug() << "text: " << questionData.text;
        qDebug() << "variants: " << questionData.variants;
    }

    return questionDatas;
}

void TestCombiner::combineQuestionDatas(TestCombiner::QuestionDatas *questionDatas)
{
    static auto deleteJunk = [](QuestionData& questionData)
    {
        questionData.text = questionData.text.remove("#question# ");
        QStringList temp;
        for (auto& variant : questionData.variants) {
            static const QRegularExpression reg("#variant# (.*?)<\\/font>");
            temp.append(Internal::getAllMatches(variant, reg));
        }
        questionData.variants = temp;
    };

    static auto addVariants = [](QuestionData& questionData, const QStringList& variants)
    {
        for (const auto& variant : variants) {
            if (questionData.variants.contains(variant))
                continue;
            questionData.variants.append(variant);
        }
    };

    QuestionDatas result;
    qDebug() << questionDatas->uniqueKeys().count();
    for (const QString& key : questionDatas->uniqueKeys()) {
        QList<QuestionData> duplicates = questionDatas->values(key);
        QuestionData questionData = duplicates.first();
        for (const QuestionData& duplicate : duplicates) {
            if (duplicate.correctAnswered) {
                questionData = duplicate;
                break;
            } else {
                addVariants(questionData, duplicate.variants);
            }
        }
        deleteJunk(questionData);
        result.insert(key, questionData);
    }

    *questionDatas = result;
}

void TestCombiner::on_combineButton_clicked()
{
    QString content = getFilesContent();
    static const QRegularExpression blockRegex("<!--{-->((?s).*?)<!--}-->");
    QStringList dataBlocks = Internal::getAllMatches(content, blockRegex);

    QuestionDatas questionDatas = getQuestionDatas(dataBlocks);
    combineQuestionDatas(&questionDatas);

    emit dataIsReady(this, "", questionDatas.values());
}
