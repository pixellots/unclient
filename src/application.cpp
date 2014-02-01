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

#include "application.h"
#include "logging.h"
#include "config.h"
#include "settings.h"
#include <QApplication>
#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTemporaryFile>
#include <QProcess>
#include <QMessageBox>

using namespace UpdateNode;

/*!
\class UpdateNode::Application
\brief Main application class which cares about application wide instructions,
like installing language, single instance when application is hidden, or relaunching on a save place
*/

/*!
Constructs a Application object.
This constructor checks the existance of the default.qss file which contains the custom stylesheet.
\n If no default.qss file is located in the current working dir, no style sheet is set.
*/
Application::Application(QObject *parent) :
    QObject(parent)
{
    QFile style("default.qss");
    if(style.exists())
    {
         if(style.open(QIODevice::ReadOnly))
         {
             qApp->setStyleSheet(style.readAll());
             style.close();
         }
    }

    m_pService = new UpdateNode::Service(0);
    m_pSystemTray = 0;
}

/*!
Destructs the Application object.
*/
Application::~Application()
{
    if(m_pSystemTray)
        delete m_pSystemTray;
}

/*!
Installs the set language as specified in Update::Config::getLanguage. This always checks for the
existance of the language file - even for english (en, en_US, etc.) \n
If the language has been successfully loaded, \a true is returned. In case of any error, false is returned.
\note This method searches for language files in this order: \l working directory \l <working_dir>/translations \l internal resource
\note the translation file needs the following naming: unclient_<language_code>.qm
*/
bool Application::installTranslations()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    QString translationFrom;
    if(!m_oTranslator.load("unclient_" + config->getLanguage()))
    {
        if(!m_oTranslator.load("translations/unclient_" + config->getLanguage()))
        {
            if(m_oTranslator.load(":/translations/unclient_" + config->getLanguage()))
                translationFrom = "*internal resource*";
        }
        else
            translationFrom = "translations";
    }
    else
        translationFrom = ".";

    qApp->installTranslator(&m_oTranslator);

    if(!translationFrom.isEmpty())
        UpdateNode::Logging() << "Translation for" << config->getLanguage() << "loaded from:" << translationFrom;

    return !translationFrom.isEmpty();
}

/*!
Shows splash screen as long as the service is operating
\sa Config::setSplashScreen
*/
void Application::showSplashScreen(UpdateNode::Service* aService, const QString& aMode)
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    if(!config->getSplashScreen().isEmpty())
    {
        m_oSplashScreen_pic.load(config->getSplashScreen());
        m_oSplashScreen.setPixmap(m_oSplashScreen_pic);
        if(aMode=="-messages")
            m_oSplashScreen.showMessage(QObject::tr("Checking for messages ..."), Qt::AlignCenter | Qt::AlignBottom);
        else
            m_oSplashScreen.showMessage(QObject::tr("Checking for updates ..."), Qt::AlignCenter | Qt::AlignBottom);
        m_oSplashScreen.show();
        QObject::connect(aService, SIGNAL(done()), &m_oSplashScreen, SLOT(close()));
        QObject::connect(aService, SIGNAL(doneManager()), &m_oSplashScreen, SLOT(close()));
    }
}

/*!
Relaunches the current client in system's temp directory. Before doing that, the launched client is copied to TMP.
Once copied, it is checking its checksum against the version in TMP. If the client already exists in TMP and the
checksum is invalid, the client in TMP gets deleted and this function is re-called.
Returns true on success, false when in, or out file cannot be read/written.
*/
bool Application::relaunchUpdateSave(const QString& aKey)
{
    QDir newClientPath(QDir::tempPath() + QDir::separator() + aKey);
    QString clientExecutable(QFileInfo(qApp->applicationFilePath()).fileName());

    QFile currentFile(qApp->applicationFilePath());
    QFile newFile(newClientPath.absolutePath() + QDir::separator() + clientExecutable);

    quint16 src, dst;

    if(newClientPath == qApp->applicationDirPath())
        return false;

    if(currentFile.open(QIODevice::ReadOnly))
    {
        QByteArray data;
        data = currentFile.readAll();
        src = qChecksum(data, data.length());
        currentFile.close();
        UpdateNode::Logging() << src;
    }
    else
        return false;

    if(!newFile.exists())
    {
        if(!newClientPath.exists())
        {
            if(!newClientPath.cdUp() || !newClientPath.mkdir(aKey))
                return false;

            newClientPath.cd(aKey);
        }
        currentFile.copy(newClientPath.absolutePath() + QDir::separator() + clientExecutable);
    }


    if(newFile.open(QIODevice::ReadOnly))
    {
        QByteArray data;
        data = newFile.readAll();
        dst = qChecksum(data, data.length());
        newFile.close();
        UpdateNode::Logging() << newFile.fileName() << dst;
    }
    else
        return false;

    if(src == dst)
        return true;
    else
    {
        if(QFile::remove(newFile.fileName()))
            return relaunchUpdateSave(aKey);
    }
    return false;

}

/*!
Relaunches the cloned client, previously created with UpdateNode::Application::relaunchUpdateSave.\n
The process is launched detached. So that the initialy called client exits immediately.
Returns true if the process was started successfully.
*/
bool Application::relaunch(const QString& aKey)
{
    QString clientExecutable(QFileInfo(qApp->applicationFilePath()).fileName());
    QString newClient(QDir::tempPath() + QDir::separator() + aKey + QDir::separator() + clientExecutable);

    QStringList args = qApp->arguments();
    args.takeFirst();
    return QProcess::startDetached(newClient, args);
}

/*!
This method checks if the current instance is already running, or not.
Returns true when the process is running already, otherwise false
*/
bool Application::isAlreadyRunning(const QString& aKey)
{
    m_oSharedMemory.setKey(aKey);
    if (!m_oSharedMemory.create(1))
    {
        connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(killMeOrNot()));
        m_oTimer.start(500);

        m_oSharedMemory.attach();
        return true;
    }

    char* data = (char*)m_oSharedMemory.data();

    if(data)
        memcpy(data, "0", 1);

    connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(killMeOrNot()));
    m_oTimer.start(500);
    return false;
}

/*!
Checks whether the process is running hidden (invisible to the user), or not.
Returns true if hidden
\sa Application::setVisible
*/
bool Application::isHidden()
{
    m_oSharedMemory.lock();

    char* data = (char*)m_oSharedMemory.data();

    if(data)
    {
        if(data[0] == 'H')
        {
            m_oSharedMemory.unlock();
            return true;
        }
    }
    m_oSharedMemory.unlock();
    return false;
}

/*!
 Sets the visible state as specified by /a aShown
\sa Application::isHidden
*/
void Application::setVisible(bool aShown)
{
    m_visible = aShown;
}

/*!
Kills the other hidden process
*/
void Application::killOther()
{
    m_oSharedMemory.lock();

    char* data = (char*)m_oSharedMemory.data();

    if(data)
        memcpy(data, "-", 1);

    m_oSharedMemory.unlock();
}

/*!
This method is killing the current process if the another process has
given the signal to terminate.
*/
void Application::killMeOrNot()
{
    m_oSharedMemory.lock();

    char* data = (char*)m_oSharedMemory.data();

    if(data)
    {
        if(data[0] == '-')
        {
            if(!m_visible)
                memcpy(data, "H", 1);
            else
                memcpy(data, "S", 1);
            qApp->quit();
        }
        else
        {
            if(!m_visible)
                memcpy(data, "H", 1);
            else
                memcpy(data, "S", 1);
        }
    }
    m_oSharedMemory.unlock();
}

/*!
Returns the error code and optionaly lauches an external program\n
as specified with -exec parameter.
\note There are two optional parameters which can be passed to your
\note lauching program.[UN_ERRORCODE] which is the returned error code
\note and [UN_VERSION] which identifies the current version.
\note (only for single mode updates)
*/
int Application::returnANDlaunch(int aResult)
{
    QString exec = UpdateNode::Config::Instance()->getExec();

    if(!exec.isEmpty())
    {
        exec = exec.replace("[UN_ERRORCODE]", QString::number(aResult));
        if(UpdateNode::Config::Instance()->isSingleMode())
        {
            UpdateNode::Settings settings;
            exec = exec.replace("[UN_VERSION]", settings.getProductVersion());
        }

        // set to "-" in case the above if statement does not match
        exec = exec.replace("[UN_VERSION]", "-");

        UpdateNode::Logging() << "Lauching: " << exec;

        if(!QProcess::startDetached(exec))
            QMessageBox::critical(0, QString("%1 %2").arg(UPDATENODE_COMPANY_STR).arg(UPDATENODE_APPLICATION_STR), QObject::tr("Unable to launch '%1'").arg(exec));
    }

    UpdateNode::Logging() << "unclient finished with: " << errorCodeToString(aResult);

    qApp->exit(aResult);

    return aResult;
}

/*!
Coverts the error code into a human readyble string
*/
QString Application::errorCodeToString(int aCode) const
{
    QString result;
    switch(aCode)
    {
        case 0:
            result = "Success";
            break;

        case UPDATENODE_PROCERROR_NO_UPDATES:
            result = "No Updates/Messages available";
            break;

        case UPDATENODE_PROCERROR_WRONG_PARAMETER:
            result = "Wrong parameter passed";
            break;

        case UPDATENODE_PROCERROR_CANCELED:
            result = "Process canceled by user";
            break;

        case UPDATENODE_PROCERROR_UPDATE_EXEC_FAILED:
            result = "Failed to execute the update";
            break;

        case UPDATENODE_PROCERROR_UPDATE_EXEC_CRASHED:
            result = "The update process crashed";
            break;

        case UPDATENODE_PROCERROR_COMMAND_LAUNCH_FAILED:
            result = "Failed to launch the update command";
            break;

        case UPDATENODE_PROCERROR_DOWNLOAD_FAILED:
            result = "Download error";
            break;

        case UPDATENODE_PROCERROR_WINDOWS_UAC_FAILED:
            result = "Couldn't get administrative rights";
            break;

        case UPDATENODE_PROCERROR_RUN_DOWNLOAD_FIRST:
            result = "Download mode (-download) needs to be finshed before execution is started";
            break;

        case UPDATENODE_PROCERROR_REGISTER_FIRST:
            result = "Register a product first";
            break;

        case UPDATENODE_PROCERROR_SERVICE_ERROR:
            result = "UpdateNode API returned error";
            break;

        default:
            result = "Unknown error";
            break;
    }
    return result + " (" + QString::number(aCode) + ")";
}


/*!
Sets the pointer to the current used UpdateNode::Service
*/
void Application::setService(Service *aService)
{
    m_pService = aService;
}

/*!
Sets the mode in which unclient is running
*/
void Application::setMode(const QString& aMode)
{
    m_strMode = aMode;
}

/*!
Main method which checks for updates and shown the
right dialog based on the running mode
*/
void Application::checkForUpdates()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    if(m_strMode != "-check")
    {
        showSplashScreen(m_pService, m_strMode);

        if(config->isSingleMode())
        {
            if(m_strMode == "-update" || m_strMode == "-download" || m_strMode == "-execute")
            {
                m_oSingleDialog.init(m_pService, m_strMode == "-download", m_strMode == "-execute");
                m_oSingleDialog.hide();
            }
            else if(m_strMode == "-messages")
            {
                m_oMessageDialog.init(m_pService);
                m_oMessageDialog.hide();
            }
            else
            {
                m_oManageDialog.init(m_pService);
                m_oManageDialog.hide();
            }
        }
        else
        {
            m_oManageDialog.init(m_pService);
            m_oManageDialog.hide();
        }
    }
    else
    {
        if(config->isSingleMode())
            QObject::connect(m_pService, SIGNAL(done()), this, SLOT(afterCheck()));
        else
            QObject::connect(m_pService, SIGNAL(doneManager()), this, SLOT(afterCheck()));
    }

    m_pService->checkForUpdates();
}

/*!
Slot which is called in case of -check mode. \n
Depends on the config, just returns using silent mode\n
or show a message box with the result.\n
This slot is ending the main event loop\n
\note When unclient is lauched with -st (Stytem Tray)
\note a system tray icon is shown from this slot
*/
void Application::afterCheck()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    if(config->isSilent())
    {
        if(config->isSingleMode())
            qApp->exit(m_pService->returnCode());
        else
            qApp->exit(m_pService->returnCodeManager());
    }
    else
    {
        QString text;

        if(config->isSingleMode())
            text = m_pService->notificationText();
        else
            text = m_pService->notificationTextManager();

        if(config->isSystemTray() && (m_pService->returnCode() != 101 || m_pService->returnCodeManager() != 101))
        {
            m_pSystemTray = new UpdateNode::SystemTray();
            QObject::connect(m_pSystemTray, SIGNAL(launchClient()), this, SLOT(setVisible()));
            QObject::connect(m_pSystemTray, SIGNAL(launchMessages()), this, SLOT(setVisible()));

            if(config->isSingleMode())
                QObject::connect(m_pSystemTray, SIGNAL(launchClient()), &m_oSingleDialog, SLOT(serviceDone()));
            else
                QObject::connect(m_pSystemTray, SIGNAL(launchClient()), &m_oManageDialog, SLOT(serviceDoneManager()));
            QObject::connect(m_pSystemTray, SIGNAL(launchMessages()), &m_oMessageDialog, SLOT(serviceDone()));

            if(config->isSingleMode())
                m_pSystemTray->actionsBasedOnReturn(m_pService->returnCode());
            else
                m_pSystemTray->actionsBasedOnReturn(m_pService->returnCodeManager());

            m_pSystemTray->showMessage(text);
            qApp->exit(0);
            return;
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
            qApp->exit(m_pService->returnCode());
        else
            qApp->exit(m_pService->returnCodeManager());
    }
}
