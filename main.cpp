#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QDebug>
#include "dialog.h"
#include "config.h"
#include "settings.h"
#include "sara_service.h"
#include "version.h"

int printHelp()
{
    QString appName = QString("%1 %2 %3").arg(SARA_COMPANY_STR).arg(SARA_APPLICATION_STR).arg(SARA_CLIENT_VERSION);

    QString message = QString("Command Line Parameters:")
            + "\n\n"
            + "-k <key>    \tUnique Sara key\n"
            + "-vc <code>  \tProduct Version Code\n"
            + "-pc <code>  \tProduct Code\n"
            + "-v <version>\tProduct Version\n"
            + "-s          \tSilent check\n";

    QMessageBox::information(NULL, appName, message);
    return 1;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sara::Config* config = Sara::Config::Instance();
    Sara::Service* service = new Sara::Service(0);
    Sara::Settings settings;

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
        else if(arguments.at(i) == "-h" || arguments.at(i) == "--h" || arguments.at(i) == "--help")
            return printHelp();
    }

    Dialog w;
    if(!config->isSilent())
    {
        w.init(service);
        w.hide();
    }
    else
        QObject::connect(service, SIGNAL(done()), &a, SLOT(quit()));

    if(config->isVersionMapped())
        qDebug() << "Version is mapped";

    service->checkForUpdates();

    return a.exec();
}
