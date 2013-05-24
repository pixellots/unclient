#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QList>

#include "product.h"
#include "productversion.h"
#include "update.h"
#include "message.h"

namespace Sara
{
    class Config
    {
        public:
            static Config* Instance();

            QString mainIcon() const;
            void setMainIcon(const QString& aIcon);

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

            int getUpdateInterval();
            void setUpdateInterval(int aInterval);

            QString getOS() const;

            void setProduct(const Product& aProduct);
            Sara::Product product();

            void setVersion(const ProductVersion& aVersion);
            Sara::ProductVersion version();

            QList<Sara::Update> updates();
            QList<Sara::Message> messages();

            void addUpdate(const Sara::Update& aUpdate);
            void addMessage(const Sara::Message& aMessage);

            void clear();

        private:
            Config();
            Config(Config const&);

        private:
            static Config* m_pInstance;

            bool m_bSilent;
            bool m_bSytemTray;
            bool m_bSingleMode;

            int m_iUpdateInterval;

            QString m_strMainIcon;
            QString m_strKey;
            QString m_strTestKey;
            QString m_strVersionCode;
            QString m_strProductCode;
            QString m_strVersion;
            QString m_strLanguage;

            Sara::Product m_oProduct;
            Sara::ProductVersion m_oCurrentVersion;

            QList<Sara::Update> m_listUpdates;
            QList<Sara::Message> m_listMessages;

    };
}

#endif // CONFIG_H
