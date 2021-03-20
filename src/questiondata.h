#ifndef QUESTIONDATA_H
#define QUESTIONDATA_H

#include <QStringList>

struct QuestionData
{
    QString text;
    QStringList variants;
    QString id;
    bool correctAnswered = true;
};

#endif // QUESTIONDATA_H
