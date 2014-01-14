/****************************************************************************
**
** Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
** Contact: code@updatenode.com
**
** This file is part of the UpdateNode Client.
**
** Commercial License Usage
** Licensees holding valid commercial UpdateNode license may use this file
** under the terms of the the Apache License, Version 2.0
** Full license description file: LICENSE.COM
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation. Please review the following information to ensure the
** GNU General Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
** Full license description file: LICENSE.GPL
**
****************************************************************************/

#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QVBoxLayout>
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
#include "helpdialog.h"

#ifndef APP_COPYRIGHT
#define APP_COPYRIGHT "(C) 2014 UpdateNode UG (haftungsbeschränkt). All rights reserved."
#endif

int printHelp()
{
    QString appName =   QString("%1 %2 %3").arg(UPDATENODE_COMPANY_STR).arg(UPDATENODE_APPLICATION_STR).arg(UPDATENODE_CLIENT_VERSION);
    QString message =   QString("Command Line Parameters: \n\n%1 <options> mode").arg(QFileInfo(qApp->arguments().at(0)).fileName())
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
            + "  -i <png_file>  \tMain PNG Icon\n"
            + "  -s             \tSilent mode\n"
            + "  -r             \tRelaunch client in temp directory (self update)\n"
            + "  -st            \tSystem Tray Icon (-check mode only)\n"
            + "  -log <file>    \tEnables Logging\n"
            + "  -config <file> \tLoads parameter settings from file\n"
            + "  -l <lang-code> \tLanguage Code\n"
            + "  -sp <png_file> \tSplash screen (PNG)\n"
            + "  -exec <command>\tLaunches command before terminating\n"
            + "  -v             \tShows current UpdateNode client version\n"
            + "\n";

#ifdef Q_OS_UNIX
    printf("%s\n%s\n\n%s", appName.toStdString().c_str(), APP_COPYRIGHT, message.toStdString().c_str());
    return 0;
#endif
    Helpdialog help;
    help.setText(appName, QString(APP_COPYRIGHT) + "\n\n" + message);
    help.exec();
    return 0;
}

int returnANDlaunch(int result)
{
    QString exec = UpdateNode::Config::Instance()->getExec();

    if(!exec.isEmpty())
    {
        UpdateNode::Logging() << "Lauching: " << exec;
        exec = exec.replace("[UN_ERRORCODE]", QString::number(result));
        if(UpdateNode::Config::Instance()->isSingleMode())
        {
            if(UpdateNode::Config::Instance()->updates().size()==1)
            {
                UpdateNode::Update update = UpdateNode::Config::Instance()->updates().at(0);
                if(update.getTypeEnum() == UpdateNode::Update::CLIENT_SETS_VERSION)
                {
                    UpdateNode::Settings settings;
                    exec = exec.replace("[UN_VERSION]", settings.getProductVersion());
                }
            }
        }

        // set to "-" in case the above if statement does not match
        exec = exec.replace("[UN_VERSION]", "-");

        if(!QProcess::startDetached(exec))
            QMessageBox::critical(0, QString("%1 %2").arg(UPDATENODE_COMPANY_STR).arg(UPDATENODE_APPLICATION_STR), QObject::tr("Unable to launch '%1'").arg(exec));
    }

    UpdateNode::Logging() << "unclient finished with return code" << QString::number(result);

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
    if(settings->contains("splash"))
        config->setSplashScreen(settings->value("splash").toString());

    delete settings;
}

int main(int argc, char *argv[])
{
    if(argc > 1 && strcmp(argv[1],"-copy") == 0)
    {
        // check for copy commmand
        QCoreApplication app(argc, argv);
        QStringList args = app.arguments();
        int index = args.indexOf("-copy");
        if(index>-1 && args.size()==4)
        {
            if(UpdateNode::Commander::copy(args.at(2), args.at(3)))
                return 0;
            else
                return -1;
        }
        return UPDATENODE_PROCERROR_WRONG_PARAMETER;
    }

    QApplication a(argc, argv);
    UpdateNode::Application un_app;

    UpdateNode::Config* config = UpdateNode::Config::Instance();
    UpdateNode::Service* service = new UpdateNode::Service(0);

    QString mode;
    QString argument;
    QStringList arguments = a.arguments();

    // get config data
    int index = arguments.indexOf("-config");
    if(index>-1 || QFile::exists("unclient.cfg"))
    {
        if(index>-1)
            getParametersFromFile(arguments.at(index+1));
        else
            getParametersFromFile("unclient.cfg");
    }

    for (int i = 0; i < arguments.size(); ++i)
    {
        argument = arguments.at(i);
        bool hasNext = (i+1) < arguments.size();
        
        if(argument == "-k" && hasNext)
            config->setKey(arguments.at(i+1));
        else if(argument == "-t" && hasNext)
            config->setTestKey(arguments.at(i+1));
        else if(argument == "-vc" && hasNext)
        {
            config->setSingleMode(true);
            config->setVersionCode(arguments.at(i+1));
        }
        else if(argument == "-pc" && hasNext)
        {
            config->setSingleMode(true);
            config->setProductCode(arguments.at(i+1));
        }
        else if(argument == "-v" && hasNext)
        {
            config->setSingleMode(true);
            config->setVersion(arguments.at(i+1));
        }
        else if(argument == "-host" && hasNext)
            config->setHost(arguments.at(i+1));
        else if(argument == "-s")
            config->setSilent(true);
        else if(argument == "-r")
            config->setRelaunch(true);
        else if(argument == "-st")
            config->setSystemTray(true);
        else if(argument == "-i" && hasNext)
        {
            config->setMainIcon(arguments.at(i+1));
            a.setWindowIcon(QPixmap(arguments.at(i+1)));
        }
        else if(arguments.at(i) == "-l" && hasNext)
            config->setLanguage(arguments.at(i+1));
        else if(arguments.at(i) == "-log" && hasNext)
            config->setLogging(arguments.at(i+1));
        else if(arguments.at(i) == "-sp" && hasNext)
            config->setSplashScreen(arguments.at(i+1));
        else if(arguments.at(i) == "-exec" && hasNext)
            config->setExec(arguments.at(i+1));
        else if(arguments.at(i) == "-h" || arguments.at(i) == "--h" || arguments.at(i) == "--help" || arguments.at(i) == "-help")
            return printHelp();
        else if(arguments.at(i) == "-v")
        {
            printf("%d.%d.%d.%d\n", APP_VERSION_HIGH, APP_VERSION_LOW, APP_VERSION_REV, APP_VERSION_BUILD);
            return 0;
        }
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

    if(mode.isEmpty() && config->isSingleMode())
        mode = "-update";
    else if(mode.isEmpty())
        mode = "-manager";

    UpdateNode::Settings settings;
    if(mode == "-register" || mode == "-unregister")
    {
        if(mode == "-register")
           return settings.registerVersion() ? 0 : 1;
        else
           return settings.unRegisterVersion() ? 0 : 1;
    }

    if(config->isRelaunch() && (mode == "-manager" || mode == "-update" || mode == "-execute") && un_app.relaunchUpdateSave(config->getKeyHashed()))
    {
        settings.setCurrentClientDir(qApp->applicationDirPath());
        un_app.relaunch(config->getKeyHashed());
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

    if(un_app.isAlreadyRunning(config->getKeyHashed()))
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

    un_app.installTranslations();

    if(mode == "-manager" || mode.isEmpty() || (mode == "-check" && !config->isSingleMode()))
    {
        if(config->getVersion().isEmpty() && config->getVersionCode().isEmpty() && config->getProductCode().isEmpty())
            settings.getRegisteredVersion();

        if(config->getVersion().isEmpty() && config->getVersionCode().isEmpty() && config->getProductCode().isEmpty() && config->configurations().size()==0)
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
        un_app.showSplashScreen(service, mode);

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

            if(config->isSystemTray() && (service->returnCode() != 101 || service->returnCodeManager() != 101))
            {
                UpdateNode::SystemTray tray;
                QObject::connect(&tray, SIGNAL(launchClient()), &un_app, SLOT(setVisible()));
                QObject::connect(&tray, SIGNAL(launchMessages()), &un_app, SLOT(setVisible()));

                if(config->isSingleMode())
                    QObject::connect(&tray, SIGNAL(launchClient()), &singleDialog, SLOT(serviceDone()));
                else
                    QObject::connect(&tray, SIGNAL(launchClient()), &manageDialog, SLOT(serviceDoneManager()));
                QObject::connect(&tray, SIGNAL(launchMessages()), &messageDialog, SLOT(serviceDone()));

                if(config->isSingleMode())
                    tray.actionsBasedOnReturn(service->returnCode());
                else
                    tray.actionsBasedOnReturn(service->returnCodeManager());

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
