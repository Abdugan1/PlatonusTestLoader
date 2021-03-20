#include "internal.h"

QStringList Internal::getAllMatches(const QString& str, const QRegularExpression& regex, int group)
{
    QStringList matchesList;
    QRegularExpressionMatchIterator it = regex.globalMatch(str);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        if (match.hasMatch())
            matchesList.push_back(match.captured(group));
    }
    return matchesList;
}
