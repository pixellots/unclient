#include "osdetection.h"
#include <QSysInfo>
#include <QProcess>
#include <QStringList>
#include <stdlib.h>

using namespace Sara;

QString OSDetection::getWindowsVersion()
{
#ifdef Q_WS_WIN
    switch(QSysInfo::WindowsVersion())
    {
        QString prefix = "Windows ";

        case QSysInfo::WV_CE:
            return prefix + "CE";
        case QSysInfo::WV_CENET:
            return prefix + "CE.NET";
        case QSysInfo::WV_CE_5:
            return prefix + "CE 5.x";
        case QSysInfo::WV_CE_6:
            return prefix + "CE 6.x";
        case QSysInfo::WV_32s:
            return prefix + "3.1";
        case QSysInfo::WV_95:
            return prefix + "95";
        case QSysInfo::WV_98:
            return prefix + "98";
        case QSysInfo::WV_Me:
            return prefix + "Me";
        case QSysInfo::WV_NT:
            return prefix + "NT";
        case QSysInfo::WV_2000:
            return prefix + "2000";
        case QSysInfo::WV_XP:
            return prefix + "XP";
        case QSysInfo::WV_2003:
            return prefix + "Server 2003/ XP x64";
        case QSysInfo::WV_VISTA:
            return prefix + "Vista";
        case QSysInfo::WV_WINDOWS7:
            return prefix + "7";
        case QSysInfo::WV_WINDOWS8:
            return prefix + "8";
        default:
            return "Unknown Windows";
    }
#else
    return "Unsupported";
#endif
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
        return  QString("Linux ") + kernelVersion + " (" + QString(getenv("DESKTOP_SESSION")) + ")";
    }
    else
        return  QString("Linux (" + QString(getenv("DESKTOP_SESSION")) + ")");
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
#ifdef Q_OS_WINDOWS
    return Sara::OSDetection::getMacVersion();
#else
#ifdef Q_OS_MAC
    return Sara::OSDetection::getMacVersion();
#else
    return Sara::OSDetection::getOthersVersion();
#endif
#endif
#endif
}

