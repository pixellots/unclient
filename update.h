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

#ifndef UPDATE_H
#define UPDATE_H

#include <QString>
#include "productversion.h"

namespace UpdateNode
{
    class Update
    {
        public:
            Update();

            enum Type { INSTALLER_SETS_VERSION = 1, CLIENT_SETS_VERSION = 2, UNDEFINED = 0 };

        public:
            void setTitle(const QString& aTitle);
            QString getTitle() const;

            void setDescription(const QString& aDescription);
            QString getDescription() const;

            void setDownloadLink(const QString& aDownloadLink);
            QString getDownloadLink() const;

            void setCommand(const QString& aCommand);
            QString getCommand() const;

            void setCommandLine(const QString& aCommandLine);
            QString getCommandLine() const;

            void setFileSize(const QString& aFileSize);
            QString getFileSize() const;

            void setType(int aType);
            int getType() const;
            Type getTypeEnum();

            void setRequiresAdmin(bool aAdminRequired);
            bool isAdminRequired() const;

            void setTargetVersion(const ProductVersion& aTarget);
            ProductVersion getTargetVersion() const;

            void setCode(const QString& aCode);
            QString getCode() const;

        private:
            QString m_strTitle;
            QString m_strDescription;
            QString m_strDownloadLink;
            QString m_strCommand;
            QString m_strCommandLine;
            QString m_strCode;

            QString m_strFileSize;
            int m_iType;
            bool m_bAdminRequired;

            ProductVersion m_oTarget;
    };
}

#endif // UPDATE_H
