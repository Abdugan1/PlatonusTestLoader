#ifndef INTERNAL_H
#define INTERNAL_H

#include <QObject>
#include <QRegularExpression>

class Internal : public QObject
{
    Q_OBJECT
public:
    static QStringList getAllMatches(const QString& str, const QRegularExpression& regex, int group = 1);
public slots:
};

#endif // INTERNAL_H
