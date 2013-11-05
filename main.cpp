#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTranslator>
#include "logging.h"
#include "application.h"
#include "multiappdialog.h"
#include "singleappdialog.h"
#include "config.h"
#include "settings.h"
#include "updatenode_service.h"
#include "version.h"
#include "status.h"
#include "usermessages.h"
#include "systemtray.h"

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
            + "  -r             \tRelaunch client in temp directory (self update)\n"
            + "  -st            \tSystem Tray Icon (-check mode only)\n"
            + "  -log <file>    \tEnables Logging\n"
            + "  -exec <command>\tLaunches command before terminating\n"
            + "  -config <file> \tLoads parameter settings from file\n";

    QMessageBox::information(NULL, appName, message);
    return 1;
}

int returnANDlaunch(int result)
{
    QString exec = UpdateNode::Config::Instance()->getExec();

    if(!exec.isEmpty())
    {
        if(!QProcess::startDetached(exec))
            QMessageBox::critical(0, "TODO", QObject::tr("Unable to launch %1").arg(exec));
    }

    return result;
}

void getParametersFromFile(const QString& file)
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();
    QSettings *settings = new QSettings(file, QSettings::IniFormat);
    if(settings->contains("key"))
        config->setKey(settings->value("key").toString());
    if(settings->contains("test_key"))
        config->setTestKey(settings->value("test_key").toString());
    if(settings->contains("version_code"))
    {
        config->setSingleMode(true);
        config->setVersionCode(settings->value("version_code").toString());
    }
    if(settings->contains("product_code"))
    {
        config->setSingleMode(true);
        config->setProductCode(settings->value("product_code").toString());
    }
    if(settings->contains("version"))
    {
        config->setSingleMode(true);
        config->setVersion(settings->value("version").toString());
    }
    if(settings->contains("icon"))
        config->setMainIcon(settings->value("icon").toString());
    if(settings->contains("language"))
        config->setLanguage(settings->value("language").toString());
    if(settings->contains("silent"))
        config->setSilent(settings->value("silent").toString().toLower()=="true");
    if(settings->contains("systemtray"))
        config->setSystemTray(settings->value("systemtray").toString().toLower()=="true");
    if(settings->contains("relaunch"))
        config->setRelaunch(settings->value("relaunch").toString().toLower()=="true");
    if(settings->contains("log"))
        config->setLogging(settings->value("log").toString());
    if(settings->contains("exec_command"))
        config->setExec(settings->value("exec_command").toString());

    delete settings;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdateNode::Application un_app;

    UpdateNode::Config* config = UpdateNode::Config::Instance();
    UpdateNode::Service* service = new UpdateNode::Service(0);

    QString mode = "-manager";
    QString argument;
    QStringList arguments = QCoreApplication::arguments();

    int config_index = arguments.indexOf("-config");
    if(config_index>-1)
        getParametersFromFile(arguments.at(config_index+1));

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
        else if(argument == "-r")
            config->setRelaunch(true);
        else if(argument == "-st")
            config->setSystemTray(true);
        else if(argument == "-i")
            config->setMainIcon(arguments.at(i+1));
        else if(arguments.at(i) == "-l")
            config->setLanguage(arguments.at(i+1));
        else if(arguments.at(i) == "-log")
            config->setLogging(arguments.at(i+1));
        else if(arguments.at(i) == "-exec")
            config->setExec(arguments.at(i+1));
        else if(arguments.at(i) == "-h" || arguments.at(i) == "--h" || arguments.at(i) == "--help" || arguments.at(i) == "-help")
            return printHelp();
        else if(argument == "-update" || argument == "-messages"
                || argument == "-register" || argument == "-unregister" || argument == "-manager"
                || argument == "-check" || argument == "-download" || argument == "-execute")
            mode = argument;
    }

    if(config->getKey().isEmpty())
        return returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);
    else if(!config->getVersion().isEmpty() && config->getProductCode().isEmpty())
        return returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);
    else if(config->getVersion().isEmpty() && !config->getProductCode().isEmpty())
        return returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);

    UpdateNode::Settings settings;
    if(mode == "-register" || mode == "-unregister")
    {
        if(mode == "-register")
           return returnANDlaunch(settings.registerVersion() ? 0 : 1);
        else
           return returnANDlaunch(settings.unRegisterVersion() ? 0 : 1);
    }

    if(config->isRelaunch() && (mode == "-manager" || mode == "-update" || mode == "-execute") && un_app.relaunchUpdateSave(config->getKey()))
    {
        settings.setCurrentClientDir(qApp->applicationDirPath());
        un_app.relaunch(config->getKey());
        return 0;
    }
    else if (mode != "-manager")
        settings.setCurrentClientDir(qApp->applicationDirPath());

    if(mode != "-manager" && mode != "-check")
    {
        if(config->getVersion().isEmpty() && config->getProductCode().isEmpty()
                && config->getVersionCode().isEmpty())
            return returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);

    }

    if(un_app.isAlreadyRunning(config->getKey()))
    {
        if(!un_app.isHidden())
            return returnANDlaunch(0);
        else
            un_app.killOther();
    }

    if(mode == "-check" && config->isSystemTray())
        un_app.setVisible(false);
    else
        un_app.setVisible();

    QTranslator translator;
    translator.load(config->getLanguage(), "translations");
    a.installTranslator(&translator);

    QFile style("default.qss");
    if(style.exists())
    {
         if(style.open(QIODevice::ReadOnly))
         {
             a.setStyleSheet(style.readAll());
             style.close();
         }
    }

    if(mode == "-manager" || mode.isEmpty() || (mode == "-check" && !config->isSingleMode()))
    {
        if(config->getVersion().isEmpty() && config->getVersionCode().isEmpty() && config->getProductCode().isEmpty())
            settings.getRegisteredVersion();

        if(config->configurations().size()==0)
        {
            UpdateNode::Logging() << "ERROR: There are no versions registered";
            return returnANDlaunch(UPDATENODE_PROCERROR_REGISTER_FIRST);
        }
    }
    UserMessages messageDialog;
    SingleAppDialog singleDialog;
    MultiAppDialog manageDialog;

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
    {
        if(config->isSingleMode())
            QObject::connect(service, SIGNAL(done()), &a, SLOT(quit()));
        else
            QObject::connect(service, SIGNAL(doneManager()), &a, SLOT(quit()));
    }

    service->checkForUpdates();

    int result = a.exec();

    if(mode != "-check")
        return returnANDlaunch(result);
    else
    {
        if(config->isSilent())
            return returnANDlaunch(service->returnCode());
        else
        {
            QString text;

            if(config->isSingleMode())
                text = service->notificationText();
            else
                text = service->notificationTextManager();

            if(config->isSystemTray() && (service->returnCode() != 0 || service->returnCodeManager() != 0))
            {
                UpdateNode::SystemTray tray;
                QObject::connect(&tray, SIGNAL(launchClient()), &un_app, SLOT(setVisible()));

                if(config->isSingleMode())
                    QObject::connect(&tray, SIGNAL(launchClient()), &singleDialog, SLOT(serviceDone()));
                else
                    QObject::connect(&tray, SIGNAL(launchClient()), &manageDialog, SLOT(serviceDoneManager()));
                tray.showMessage(text);
                int tray_res = a.exec();
                tray.hide();
                return returnANDlaunch(tray_res);
            }
            else if(!config->isSystemTray())
            {
                if(config->mainIcon().isEmpty())
                    qApp->setWindowIcon(QIcon(config->product().getLocalIcon()));
                else
                    qApp->setWindowIcon(QIcon(config->mainIcon()));

                QMessageBox::information(NULL, config->product().getName(), text);
            }

            if(config->isSingleMode())
                return returnANDlaunch(service->returnCode());
            else
                return returnANDlaunch(service->returnCodeManager());
        }
    }
}
