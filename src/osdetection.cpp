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

/*!
\class UpdateNode::OSDetection
\brief OSDetection returns the informationa about the used OS
*/

/*!
Returns the used Windows version, like "Windows 6.2", or "Unknown Windows" when the version \n
information cannot be accessed
*/
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

/*!
Returns the used Mac version, like stored in /System/Library/CoreServices/SystemVersion.plist, \n
reading the ProductVersion key. If no such key is present, "Unknown Mac" is returned
*/
QString OSDetection::getMacVersion()
{
    QString prefix = "Mac OSX ";
    QString version;

    QSettings settings("/System/Library/CoreServices/SystemVersion.plist", QSettings::NativeFormat);

    version = settings.value("ProductVersion", "Unknown Mac").toString();

    if(version != "unknown")
        return prefix + version;
    else
        return version;
}

/*!
Returns the used Linux version, like returned when executing  "uname -r". Additionally, \n
Env variable DESKTOP_SESSION is taken. If there is no information available about the current version \n
"Linux (unknown)" is returned.
*/
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


/*!
Returns "Unsupported OS"
*/
QString OSDetection::getOthersVersion()
{
    return "Unsupported OS";
}

/*!
Returns the OS name based on the used system, calling the following methods:
\sa UpdateNode::OSDetection::getLinuxVersion
\sa UpdateNode::OSDetection::getWindowsVersion
\sa UpdateNode::OSDetection::getMacVersion
\sa UpdateNode::OSDetection::getOthersVersion
*/
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

/*!
Returns the processor architecture of the used OS \n
Default value for Windows is x86, for Mac nad Linux its "unknown"
*/
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
