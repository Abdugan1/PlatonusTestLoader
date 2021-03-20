#ifndef TESTCOMBINER_H
#define TESTCOMBINER_H

#include <QWidget>

namespace Ui {
class TestCombiner;
}

struct QuestionData;

class TestCombiner : public QWidget
{
    Q_OBJECT

public:
    explicit TestCombiner(QWidget *parent = nullptr);
    ~TestCombiner();
signals:
    void dataIsReady(QWidget* parent, const QString& fileName
                     , const QList<QuestionData>& questionDataList);
private slots:
    void on_openFilesButton_clicked();
    void on_combineButton_clicked();
private:
    void deleteAllFileNames();
    QString getFilesContent();
    QList<QuestionData> getQuestionDatas(const QStringList& dataBloks);
    Ui::TestCombiner *ui;
    QStringList filePaths_;
};

#endif // TESTCOMBINER_H
