#include "platonustestloaderapp.h"
#include "networkaccessmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PlatonusTestLoaderApp platonusTestLoaderApp;

    return app.exec();
}
