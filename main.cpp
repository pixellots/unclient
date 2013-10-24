#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDebug>
#include "application.h"
#include "dialog.h"
#include "singleappdialog.h"
#include "config.h"
#include "settings.h"
#include "updatenode_service.h"
#include "version.h"
#include "status.h"
#include "usermessages.h"

int printHelp()
{
    QString appName = QString("%1 %2 %3").arg(UPDATENODE_COMPANY_STR).arg(UPDATENODE_APPLICATION_STR).arg(UPDATENODE_CLIENT_VERSION);

    QString message = QString("Command Line Parameters: \n\n%1 <options> mode").arg(QFileInfo(qApp->arguments().at(0)).fileName())
            + "\n\n"
            + "Mode:\n\n"
            + "  -check          \tchecks for update\n"
            + "  -update         \truns update mode only\n"
            + "  -download       \truns update mode only, but exits after download\n"
            + "  -execute        \texecutes the downloaded update (relates to -download)\n"
            + "  -messages       \truns message mode only\n"
            + "  -manager        \truns update & message mode\n"
            + "  -register       \tregistrates the current version\n"
            + "  -unregister     \tunregistrates the current version\n"
            + "\n\n"
            + "Options:\n\n"
            + "  -k <key>       \tUnique UpdateNode key\n"
            + "  -t <key>       \tUnique UpdateNode test key\n"
            + "  -vc <code>     \tProduct Version Code\n"
            + "  -pc <code>     \tProduct Code\n"
            + "  -v <version>   \tProduct Version\n"
            + "  -i <file>      \tMain Icon\n"
            + "  -l <lang-code> \tLanguage Code\n"
            + "  -s             \tSilent check\n"
            + "  -q             \tDo not show any question dialog before\n"
            + "  -d             \tDialog when updates available\n"
            + "  -st            \tSystem Tray Icon (For -check mode, updates only)\n"
            + "  -l <lang-code> \tLanguage Code\n";

    QMessageBox::information(NULL, appName, message);
    return 1;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdateNode::Application un_app;

    UpdateNode::Config* config = UpdateNode::Config::Instance();
    UpdateNode::Service* service = new UpdateNode::Service(0);

    QString mode;
    QString argument;
    QStringList arguments = QCoreApplication::arguments();
    for (int i = 0; i < arguments.size(); ++i)
    {
        argument = arguments.at(i);
        
        if(argument == "-k")
            config->setKey(arguments.at(i+1));
        else if(argument == "-t")
            config->setTestKey(arguments.at(i+1));
        else if(argument == "-vc")
        {
            config->setSingleMode(true);
            config->setVersionCode(arguments.at(i+1));
        }
        else if(argument == "-pc")
        {
            config->setSingleMode(true);
            config->setProductCode(arguments.at(i+1));
        }
        else if(argument == "-v")
        {
            config->setSingleMode(true);
            config->setVersion(arguments.at(i+1));
        }
        else if(argument == "-host")
            config->setHost(arguments.at(i+1));
        else if(argument == "-s")
            config->setSilent(TRUE);
        else if(argument == "-st")
            config->setSystemTray(true);
        else if(argument == "-i")
            config->setMainIcon(arguments.at(i+1));
        else if(argument == "-n")
            config->setUpdateInterval(arguments.at(i+1).toInt());
        else if(arguments.at(i) == "-l")
            config->setLanguage(arguments.at(i+1));
        else if(arguments.at(i) == "-h" || arguments.at(i) == "--h" || arguments.at(i) == "--help")
            return printHelp();
        else if(argument == "-update" || argument == "-messages"
                || argument == "-register" || argument == "-unregister" || argument == "-manager"
                || argument == "-check" || argument == "-download" || argument == "-execute")
            mode = argument;
    }

    if(config->getKey().isEmpty())
        return UPDATENODE_PROCERROR_WRONG_PARAMETER;
    else if(!config->getVersion().isEmpty() && config->getProductCode().isEmpty())
        return UPDATENODE_PROCERROR_WRONG_PARAMETER;
    else if(config->getVersion().isEmpty() && !config->getProductCode().isEmpty())
        return UPDATENODE_PROCERROR_WRONG_PARAMETER;


    if(un_app.isAlreadyRunning(config->getKey()))
    {
        if(!un_app.isSystemTrayHidden())
            return 0;
        else
            un_app.killOther();
    }

    UpdateNode::Settings settings;
    if(mode == "-register" || mode == "-unregister")
    {
        if(mode == "-register")
            return settings.registerVersion() ? 0 : 1;
        else
            return settings.unRegisterVersion() ? 0 : 1;
    }
    else if(mode == "-manager" || mode.isEmpty())
    {
        if(config->getVersion().isEmpty() && config->getVersionCode().isEmpty() && config->getProductCode().isEmpty())
            settings.getRegisteredVersion();
    }
    UserMessages messageDialog;
    SingleAppDialog singleDialog;
    Dialog manageDialog;

    if(mode != "-check")
    {
        if(config->isSingleMode())
        {
            if(mode == "-update" || mode == "-download" || mode == "-execute")
            {
                singleDialog.init(service, mode == "-download", mode == "-execute");
                singleDialog.hide();
            }
            else if(mode == "-messages")
            {
                messageDialog.init(service);
                messageDialog.hide();
            }
            else
            {
                manageDialog.init(service);
                manageDialog.hide();
            }
        }
        else
        {
            manageDialog.init(service);
            manageDialog.hide();
        }
    }
    else
        QObject::connect(service, SIGNAL(done()), &a, SLOT(quit()));

    service->checkForUpdates();

    int result = a.exec();

    if(mode != "-check")
        return result;
    else
    {
        if(config->isSilent())
            return service->returnCode();
        else
        {
            QString text = service->notificationText();

            if(config->isSystemTray() && service->returnCode() != 0)
            {
                QMenu menu;
                QSystemTrayIcon tray;
                tray.setContextMenu(&menu);
                if(config->mainIcon().isEmpty())
                    tray.setIcon(QIcon(config->product().getLocalIcon()));
                else
                    tray.setIcon(QIcon(config->mainIcon()));
                tray.show();
                tray.showMessage(config->product().getName(), text);
                if(config->isSingleMode())
                {
                    QObject::connect(menu.addAction(QObject::tr("Launch Update Client")), SIGNAL(triggered()), &singleDialog, SLOT(serviceDone()));
                    QObject::connect(&tray, SIGNAL(messageClicked()), &singleDialog, SLOT(serviceDone()));
                }
                else
                {
                    QObject::connect(menu.addAction(QObject::tr("Launch Update Client")), SIGNAL(triggered()), &manageDialog, SLOT(serviceDone()));
                    QObject::connect(&tray, SIGNAL(messageClicked()), &manageDialog, SLOT(serviceDone()));
                }

                menu.addSeparator();
                QObject::connect(menu.addAction(QObject::tr("Close")), SIGNAL(triggered()), &a, SLOT(quit()));

                int tray_res = a.exec();
                tray.hide();
                return tray_res;
            }
            else if(!config->isSystemTray())
                QMessageBox::information(NULL, config->product().getName(), text);
            return service->returnCode();
        }
    }
}
