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

#ifndef WINCOMMANDER_H
#define WINCOMMANDER_H

#include <QString>
#include <QStringList>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#include <sddl.h>
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#endif

namespace UpdateNode
{
    class WinCommander
    {
    public:
        static bool isProcessElevated();
        static uint runProcessElevated(const QString &path,
                                       const QStringList &parameters = QStringList(),
                                       const QString &workingDir = QString(),
                                       bool aWait = true);
        static uint runProcessElevated(const QString &path,
                                       const QString &parameters = QString(),
                                       const QString &workingDir = QString(),
                                       bool aWait = true);

    #ifdef Q_OS_WIN
        static bool isVista();
        static bool getCurrentUserStrSid(LPTSTR *pStrSid);
    private:
        // "A" prefix, otherwise we need the Vista Platform SDK ..
        typedef enum _ATOKEN_INFORMATION_CLASS {
            ATokenUser = 1,
            ATokenGroups,
            ATokenPrivileges,
            ATokenOwner,
            ATokenPrimaryGroup,
            ATokenDefaultDacl,
            ATokenSource,
            ATokenType,
            ATokenImpersonationLevel,
            ATokenStatistics,
            ATokenRestrictedSids,
            ATokenSessionId,
            ATokenGroupsAndPrivileges,
            ATokenSessionReference,
            ATokenSandBoxInert,
            ATokenAuditPolicy,
            ATokenOrigin,
            ATokenElevationType,
            ATokenLinkedToken,
            ATokenElevation,
            ATokenHasRestrictions,
            ATokenAccessInformation,
            ATokenVirtualizationAllowed,
            ATokenVirtualizationEnabled,
            ATokenIntegrityLevel,
            ATokenUIAccess,
            ATokenMandatoryPolicy,
            ATokenLogonSid,
            AMaxTokenInfoClass  // MaxTokenInfoClass should always be the last enum
        } ATOKEN_INFORMATION_CLASS, *PATOKEN_INFORMATION_CLASS;

        typedef enum _ATOKEN_ELEVATION_TYPE {
            ATokenElevationTypeDefault = 1,
            ATokenElevationTypeFull,
            ATokenElevationTypeLimited,
        } ATOKEN_ELEVATION_TYPE, *PATOKEN_ELEVATION_TYPE;

        typedef struct _ATOKEN_ELEVATION {
            DWORD TokenIsElevated;
        } ATOKEN_ELEVATION, *PATOKEN_ELEVATION;
    #endif
    };
}

#endif // WINCOMMANDER_H
