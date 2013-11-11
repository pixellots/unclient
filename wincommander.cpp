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

#include "wincommander.h"
#include "status.h"

#include <QCoreApplication>
#include <QSysInfo>
#include <QProcess>
#include <QDir>

using namespace UpdateNode;

uint WinCommander::runProcessElevated(const QString &path,
                                              const QStringList &parameters,
                                              const QString &workingDir, bool aWait)
{
    uint result = 0;

#ifdef Q_OS_WIN
    QString params;
    HWND hwnd = NULL;
    LPCTSTR pszPath = path.utf16();
    params = parameters.join(" ");
    LPCTSTR pszParameters = params.utf16();

    QString dir;
    if (workingDir.isEmpty())
        dir = QDir::toNativeSeparators(QDir::currentPath());
    else
        dir = QDir::toNativeSeparators(workingDir);
    LPCTSTR pszDirectory = dir.utf16();

    SHELLEXECUTEINFO shex;
    DWORD dwCode  =   0;

    ZeroMemory(&shex, sizeof(shex));

    shex.cbSize       = sizeof(shex);
    shex.fMask        = SEE_MASK_NOCLOSEPROCESS;
    shex.hwnd         = hwnd;
    shex.lpVerb       = TEXT("runas");
    shex.lpFile       = pszPath;
    shex.lpParameters = pszParameters;
    shex.lpDirectory  = pszDirectory;
    shex.nShow        = SW_NORMAL;

    ShellExecuteEx(&shex);
    if (shex.hProcess)
    {
        if(aWait)
        {
            WaitForSingleObject(shex.hProcess, INFINITE );
            GetExitCodeProcess(shex.hProcess, &dwCode);
        }
        CloseHandle (shex.hProcess) ;
    }
    else
    {
        qApp->exit(UPDATENODE_PROCERROR_WINDOWS_UAC_FAILED);
        return 0;
    }

    result = (uint)dwCode;
#else
    Q_UNUSED(path);
    Q_UNUSED(parameters);
    Q_UNUSED(workingDir);
    Q_UNUSED(aWait);
#endif
    return result;
}

uint WinCommander::runProcessElevated(const QString &path, const QString &parameters, const QString &workingDir, bool aWait)
{
    return runProcessElevated(path, QStringList() << parameters, workingDir, aWait);
}

bool WinCommander::isProcessElevated()
{
    bool result = false;

#ifdef Q_OS_WIN
    HANDLE hToken = NULL;
    ATOKEN_ELEVATION te;
    DWORD dwReturnLength = 0;

    if (!isVista())
    {
        return true;
    }

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        return false;
    }

    //GetTokenInformation(hToken, TOKEN_INFORMATION_CLASS::ATokenElevationType, &elevationType, sizeof(elevationType), &dwReturnLength);
    if (!GetTokenInformation(hToken, (TOKEN_INFORMATION_CLASS)ATokenElevation, &te, sizeof(te), &dwReturnLength))
    {
        CloseHandle(hToken);
        return false;
    }
    else
    {
        result = te.TokenIsElevated;
    }

    CloseHandle(hToken);
#else
    QProcess suDetector(0);
    // if this commands returns root id (0), we are admins
    suDetector.start("id -u");
    suDetector.waitForFinished();
    QString commandResult(suDetector.readAll());
    if (commandResult.toInt() != 0 || suDetector.exitStatus() == QProcess::CrashExit)
    {
        result = false;
    }
    else
    {
        result = true;
    }
#endif

    return result;
}


#ifdef Q_OS_WIN
bool WinCommander::getCurrentUserStrSid(LPTSTR* pStrSid)
{
    bool result = false;

    // Pseudohandle so don't need to close it
    HANDLE hProcess = ::GetCurrentProcess();
    HANDLE hToken = NULL;
    if(::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
        // Get the required size
        DWORD tusize = 0;
        GetTokenInformation(hToken, TokenUser, NULL, 0, &tusize);
        TOKEN_USER* ptu = (TOKEN_USER*)new BYTE[tusize];

        if(GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, tusize, &tusize)) {
            result = ConvertSidToStringSid((SID *)ptu->User.Sid, pStrSid);
        }

        CloseHandle(hToken);
        delete [] ptu;
    }
    return result;
}

bool WinCommander::isVista()
{
    if (QSysInfo::windowsVersion() <= QSysInfo::WV_VISTA)
        return false;
    else
        return true;
}
#endif
