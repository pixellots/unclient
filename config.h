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

        public:
            Config();

        private:
            static Config* m_pInstance;

            bool m_bSilent;
            bool m_bSytemTray;
            bool m_bSingleMode;

            int m_iUpdateInterval;

            QString m_strHost;
            QString m_strMainIcon;
            QString m_strKey;
            QString m_strTestKey;
            QString m_strVersionCode;
            QString m_strProductCode;
            QString m_strVersion;
            QString m_strLanguage;
            QString m_strLogging;

            UpdateNode::Product m_oProduct;
            UpdateNode::ProductVersion m_oCurrentVersion;

            QList<UpdateNode::Update> m_listUpdates;
            QList<UpdateNode::Message> m_listMessages;
            QList<UpdateNode::Config*> m_listConfigs;

    };
}

#endif // CONFIG_H
