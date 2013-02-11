#include "osdetection.h"
#include <QSysInfo>
#include <QProcess>
#include <QStringList>
#include <QProcessEnvironment>
#include <stdlib.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

using namespace Sara;

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
#ifdef Q_WS_MAC
    switch(QSysInfo::MacintoshVersion())
    {
        QString prefix = "Mac OSX ";

        case QSysInfo::MV_CEETAH:
            return prefix + "Ceetah (10.0)";
        case QSysInfo::MV_PUMA:
            return prefix + "Puma (10.1)";
        case QSysInfo::MV_JAGUAR:
            return prefix + "Jaguar (10.2)";
        case QSysInfo::MV_PANTHER:
            return prefix + "Panther (10.3)";
        case QSysInfo::MV_TIGER:
            return prefix + "Tiger (10.4)";
        case QSysInfo::MV_LEOPARD:
            return prefix + "Leopard (10.5)";
        case QSysInfo::MV_SNOWLEOPARD:
            return prefix + "Snow Leopard (10.6)";
        case QSysInfo::MV_LION:
            return prefix + "LION (10.7)";
        default:
            return "Unknown Mac";
    }
#else
    return "Unsupported";
#endif
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
    return Sara::OSDetection::getLinuxVersion();
#else
#ifdef Q_OS_WIN
    return Sara::OSDetection::getWindowsVersion();
#else
#ifdef Q_OS_MAC
    return Sara::OSDetection::getMacVersion();
#else
    return Sara::OSDetection::getOthersVersion();
#endif
#endif
#endif
}

QString OSDetection::getArch()
{
#ifdef Q_OS_WIN
    return QProcessEnvironment::systemEnvironment().value("PROCESSOR_ARCHITECTURE", "x86");
#else
    QProcess proc;
    proc.start("uname -p");

    proc.waitForFinished(-1);

    if(proc.exitCode()==0)
        return QString(proc.readAll());
    else
        return "unknown";
#endif
}
