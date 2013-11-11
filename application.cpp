/****************************************************************************
**
** Copyright (C) 2013 UpdatNode UG.
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
#include <QApplication>
#include "logging.h"
#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTemporaryFile>
#include <QProcess>

using namespace UpdateNode;

Application::Application(QObject *parent) :
    QObject(parent)
{
}

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

bool Application::relaunch(const QString& aKey)
{
    QString clientExecutable(QFileInfo(qApp->applicationFilePath()).fileName());
    QString newClient(QDir::tempPath() + QDir::separator() + aKey + QDir::separator() + clientExecutable);

    QStringList args = qApp->arguments();
    args.takeFirst();
    return QProcess::startDetached(newClient, args);
}

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

void Application::setVisible(bool aShown)
{
    m_visible = aShown;
}

void Application::killOther()
{
    m_oSharedMemory.lock();

    char* data = (char*)m_oSharedMemory.data();

    if(data)
        qMemCopy(data, "-", 1);

    m_oSharedMemory.unlock();
}

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
