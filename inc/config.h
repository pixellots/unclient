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

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QList>

#include "product.h"
#include "productversion.h"
#include "update.h"
#include "message.h"

namespace UpdateNode
{
    class Config
    {
        public:
            static Config* Instance();

            QString mainIcon() const;
            void setMainIcon(const QString& aIcon);

            void setHost(const QString& aHost);
            QString getHost() const;

            void setKey(const QString& aKey);
            QString getKey() const;
            QString getKeyHashed() const;

            void setTestKey(const QString& aTestKey);
            QString getTestKey() const;
            void setVersionCode(const QString& aVersionCode);
            QString getVersionCode() const;
            void setProductCode(const QString& aProductCode);
            QString getProductCode() const;
            void setVersion(const QString& aVersion);
            QString getVersion() const;
            void setLanguage(const QString& aLanguage);
            QString getLanguage() const;

            void setSilent(bool aSilent);
            bool isSilent();

            void setSingleMode(bool aSingleMode);
            bool isSingleMode();

            void setSystemTray(bool aTray);
            bool isSystemTray();

            bool isEnforceMessages();
            void setEnforceMessages(bool aEnforceMessages);

            QString getOS() const;

            void setProduct(const Product& aProduct);
            UpdateNode::Product product();

            void setVersion(const ProductVersion& aVersion);
            UpdateNode::ProductVersion version();

            QList<UpdateNode::Update> updates();
            QList<UpdateNode::Message> messages();

            void addUpdate(const UpdateNode::Update& aUpdate);
            void addMessage(const UpdateNode::Message& aMessage);

            void clear();

            QList<UpdateNode::Config*> configurations();
            void addConfiguration(UpdateNode::Config* aConfig);
            void clearConfigurations();

            void setLogging(const QString& aFileName);
            bool isLoggingEnabled();
            QString getLoggingFile();

            void setExec(const QString& aFileName);
            QString getExec();

            void setRelaunch(bool aEnable);
            bool isRelaunch();

            void setSplashScreen(const QString& aFileName);
            QString getSplashScreen();

            void setStyleSheet(const QString& aFileName);
            QString getStyleSheet();

            void setTimeOut(int aTimeOutInSeconds);
            int getTimeOut();

            void setCustomRequestValue(const QString& aValue);
            QString getCustomRequestValue();

            void getParametersFromFile(const QString& aFile);
            void setParametersToFile(const QString& aFile, bool aAll = true);

        public:
            Config();

        private:
            static Config* m_pInstance;

            bool m_bSilent;
            bool m_bSytemTray;
            bool m_bSingleMode;
            bool m_bRelaunch;
            bool m_bEnforeMessages;

            QString m_strHost;
            QString m_strMainIcon;
            QString m_strKey;
            QString m_strTestKey;
            QString m_strVersionCode;
            QString m_strProductCode;
            QString m_strVersion;
            QString m_strLanguage;
            QString m_strLogging;
            QString m_strExec;
            QString m_strSplashImage;
            QString m_strStyleSheet;
            QString m_strCustomRequestValue;
            int     m_iTimeOut;

            UpdateNode::Product m_oProduct;
            UpdateNode::ProductVersion m_oCurrentVersion;

            QList<UpdateNode::Update> m_listUpdates;
            QList<UpdateNode::Message> m_listMessages;
            QList<UpdateNode::Config*> m_listConfigs;

    };
}

#endif // CONFIG_H
