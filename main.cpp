#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include "dialog.h"
#include "config.h"
#include "settings.h"
#include "sara_service.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sara::Config* config = Sara::Config::Instance();
    Sara::Settings settings;
    Sara::Service* service = new Sara::Service(0);

    QStringList arguments = QCoreApplication::arguments();
    for (int i = 0; i < arguments.size(); ++i)
    {
        if(arguments.at(i) == "-k")
            config->setKey(arguments.at(i+1));
        else if(arguments.at(i) == "-vc")
            config->setVersionCode(arguments.at(i+1));
        else if(arguments.at(i) == "-pc")
            config->setProductCode(arguments.at(i+1));
        else if(arguments.at(i) == "-v")
            config->setVersion(arguments.at(i+1));
        else if(arguments.at(i) == "-s")
            config->setSilent(TRUE);
    }

    Dialog w;
    if(!config->isSilent())
    {
        w.init(service);
        w.hide();
    }
    else
        QObject::connect(service, SIGNAL(done()), &a, SLOT(quit()));

    service->checkForUpdates();

    return a.exec();
}
