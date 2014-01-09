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
#include <QApplication>
#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTemporaryFile>
#include <QProcess>

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
        qMemCopy(data, "0", 1);

    connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(killMeOrNot()));
    m_oTimer.start(500);
    return false;
}

/*!
Checks whether the process is running hidden (unvisible to the user), or not.
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
        qMemCopy(data, "-", 1);

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
                qMemCopy(data, "H", 1);
            else
                qMemCopy(data, "S", 1);
            qApp->quit();
        }
        else
        {
            if(!m_visible)
                qMemCopy(data, "H", 1);
            else
                qMemCopy(data, "S", 1);
        }
    }
    m_oSharedMemory.unlock();
}
