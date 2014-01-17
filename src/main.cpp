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
#include <QTranslator>
#include "logging.h"
#include "application.h"
#include "config.h"
#include "settings.h"
#include "version.h"
#include "status.h"
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
            + "  -update         \truns single update mode only\n"
            + "  -download       \truns single update mode only, but exits after download\n"
            + "  -execute        \texecutes the downloaded single update (relates to -download)\n"
            + "  -messages       \truns message mode only\n"
            + "  -manager        \truns update manager mode\n"
            + "  -register       \tregistrates the current version\n"
            + "  -unregister     \tunregistrates the current version\n"
            + "\n\n"
            + "Options:\n\n"
            + "  -k <key>       \tunique UpdateNode key\n"
            + "  -t <key>       \tunique UpdateNode test key\n"
            + "  -vc <code>     \tproduct version code\n"
            + "  -pc <code>     \tproduct code\n"
            + "  -v <version>   \tproduct version\n"
            + "  -i <img_file>  \tmain icon\n"
            + "  -s             \tsilent mode\n"
            + "  -r             \trelaunch client in temp directory (self update)\n"
            + "  -st            \tsystem tray icon (-check mode only)\n"
            + "  -log <file>    \tenables logging\n"
            + "  -config <file> \tloads parameter settings from file\n"
            + "  -l <lang-code> \tlanguage code\n"
            + "  -sp <png_file> \tsplash screen (PNG)\n"
            + "  -exec <command>\tlaunches command before terminating\n"
            + "  -v             \tshows current UpdateNode client version\n"
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
        UpdateNode::Logging() << "Error -copy called with: " << app.arguments().join(" ");
        return UPDATENODE_PROCERROR_WRONG_PARAMETER;
    }

    QApplication a(argc, argv);
    UpdateNode::Application un_app;

    UpdateNode::Config* config = UpdateNode::Config::Instance();

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
        return un_app.returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);
    else if(!config->getVersion().isEmpty() && config->getProductCode().isEmpty())
        return un_app.returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);
    else if(config->getVersion().isEmpty() && !config->getProductCode().isEmpty())
        return un_app.returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);

    if(mode.isEmpty() && config->isSingleMode())
        mode = "-update";
    else if(mode.isEmpty())
        mode = "-manager";

    un_app.setMode(mode);

    UpdateNode::Settings settings;
    if(mode == "-register" || mode == "-unregister")
    {
        if(mode == "-register")
           return settings.registerVersion() ? 0 : 1;
        else
           return settings.unRegisterVersion() ? 0 : 1;
    }

    settings.setCurrentClientDir(qApp->applicationDirPath());

    if(config->isRelaunch() && (mode == "-manager" || mode == "-update" || mode == "-execute") && un_app.relaunchUpdateSave(config->getKeyHashed()))
    {
        un_app.relaunch(config->getKeyHashed());
        return 0;
    }

    if(mode != "-manager" && mode != "-check")
    {
        if(config->getVersion().isEmpty() && config->getProductCode().isEmpty()
                && config->getVersionCode().isEmpty())
            return un_app.returnANDlaunch(UPDATENODE_PROCERROR_WRONG_PARAMETER);
    }

    if(un_app.isAlreadyRunning(config->getKeyHashed()))
    {
        if(!un_app.isHidden())
            return un_app.returnANDlaunch(0);
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
            return un_app.returnANDlaunch(UPDATENODE_PROCERROR_REGISTER_FIRST);
        }
    }

    un_app.checkForUpdates();

    return a.exec();
}
