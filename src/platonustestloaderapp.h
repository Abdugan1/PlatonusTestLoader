#ifndef PLATONUSTESTLOADERAPP_H
#define PLATONUSTESTLOADERAPP_H

#include "authorization.h"
#include "platonustestloader.h"
#include "testcombiner.h"

#include <QObject>

class NetworkAccessManager;

class PlatonusTestLoaderApp : public QObject
{
    Q_OBJECT
public:
    explicit PlatonusTestLoaderApp(QObject *parent = nullptr);

signals:
public slots:
    void saveFile(QWidget* parent, const QString& fileName, const QList<QuestionData>& questionDataList);
private slots:
    void onSuccessAuthorization();
    void onLogOut();
private:
    NetworkAccessManager* networkCtrl_;

    Authorization authorization_;
    PlatonusTestLoader platonusTestLoader_;
    TestCombiner testCombiner_;
};

#endif // PLATONUSTESTLOADERAPP_H
