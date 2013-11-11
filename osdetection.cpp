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

#include "osdetection.h"
#include <QSysInfo>
#include <QProcess>
#include <QStringList>
#include <QProcessEnvironment>
#include <QSettings>
#include <stdlib.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

using namespace UpdateNode;

QString OSDetection::getWindowsVersion()
{
    QString prefix = "Windows ";

#ifdef Q_OS_WIN
    OSVERSIONINFO osinfo;

    osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if (GetVersionEx(&osinfo))
    {

        int major = osinfo.dwMajorVersion;
        int minor = osinfo.dwMinorVersion;

        return prefix + QString("%1.%2").arg(major).arg(minor);
    }
#endif
    return "Unknown Windows";

}

QString OSDetection::getMacVersion()
{
    QString prefix = "Mac OSX ";
    QString version;

    QSettings settings("/System/Library/CoreServices/SystemVersion.plist", QSettings::NativeFormat);

    version = settings.value("ProductVersion", "unknown").toString();

    if(version != "unknown")
        return prefix + version;
    else
        return version;
}

QString OSDetection::getLinuxVersion()
{
    QStringList fullKernelVersion;
    QProcess proc;
    proc.start("uname -r");

    proc.waitForFinished(-1);

    if(proc.exitCode()==0)
    {
        fullKernelVersion = QString(proc.readAll()).split(".");
        QString kernelVersion = fullKernelVersion.at(0) + "." + fullKernelVersion.at(1);
        return  QString("Linux ") + kernelVersion + " (" + QString( QProcessEnvironment::systemEnvironment().value("DESKTOP_SESSION", "unknown")) + ")";
    }
    else
        return  QString("Linux (" + QString( QProcessEnvironment::systemEnvironment().value("DESKTOP_SESSION", "unknown")) + ")");
}

QString OSDetection::getOthersVersion()
{
    return "unsupported";
}

QString OSDetection::getOS()
{
#ifdef Q_OS_LINUX
    return UpdateNode::OSDetection::getLinuxVersion();
#else
#ifdef Q_OS_WIN
    return UpdateNode::OSDetection::getWindowsVersion();
#else
#ifdef Q_OS_MACX
    return UpdateNode::OSDetection::getMacVersion();
#else
    return UpdateNode::OSDetection::getOthersVersion();
#endif
#endif
#endif
}

QString OSDetection::getArch()
{
#ifdef Q_OS_WIN
    return QProcessEnvironment::systemEnvironment().value("PROCESSOR_ARCHITECTURE", "x86");
#else
#ifdef Q_OS_LINUX
    QProcess proc;
    proc.start("uname -m");

    proc.waitForFinished(-1);

    if(proc.exitCode()==0)
        return QString(proc.readAll());
    else
        return "unknown";
#else
    QProcess proc;
    proc.start("uname -p");

    proc.waitForFinished(-1);

    if(proc.exitCode()==0)
        return QString(proc.readAll());
    else
        return "unknown";
#endif
#endif
}
