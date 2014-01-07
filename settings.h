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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include "update.h"
#include "message.h"
#include "product.h"
#include "config.h"

#define UPDATENODE_COMPANY_STR        "UpdateNode"
#define UPDATENODE_APPLICATION_STR    "Client"

namespace UpdateNode
{
    class Settings : public QSettings
    {
        public:
            Settings();

        public:
            void setDownloadPath(const QString& aPath);
            QString getDownloadPath() const;

            QString uuid();

            bool registerVersion();
            bool unRegisterVersion();
            void getRegisteredVersion();

            void setUpdate(UpdateNode::Update aUpdate, const QString& aLocalFile, int aResult);
            void setMessage(UpdateNode::Message aMessage, bool aShown, bool aLoaded);
            void setMessage(UpdateNode::Message aMessage, bool aShown);
            void setNewVersion(UpdateNode::Config* config, UpdateNode::Product aProduct, UpdateNode::ProductVersion aVersion);

            QString getProductCode(UpdateNode::Config* aConfig = NULL);
            QString getProductVersion(UpdateNode::Config* aConfig = NULL);
            QString getVersionCode(UpdateNode::Config* aConfig = NULL);


            bool messageShownAndLoaded(const QString& aMessageCode);

            void setCachedFile(const QString& aCode, const QString& aFilename);
            QString getCachedFile(const QString& aCode);

            void setCurrentClientDir(const QString& aClientDir);
            QString getCurrentClientDir();

        private:
            bool isVersionMapped(const QString& aProductCode, const QString& aVersion);
            bool isVersionMapped(const QString& aVersionCode);

            QString getMappedProductCode() const;
            QString getMappedVersionCode() const;
            QString getMappedVersion() const;


        private:
            QString m_strDownloadPath;
            QString m_strClientPath;
            QString m_strUUID;
            QString m_strUpdate;
            QString m_strMessage;
            QString m_strCurrentVersion;
            QString m_strRegistrations;

            QString m_strMappedProductCode;
            QString m_strMappedVersionCode;
            QString m_strMappedVersion;
    };
}

#endif // SETTINGS_H
