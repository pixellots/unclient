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
            void setVersionCode(const QString& aVersionCode);
            QString getVersionCode() const;
            void setProductCode(const QString& aProductCode);
            QString getProductCode() const;
            void setVersion(const QString& aVersion);
            QString getVersion() const;

            void setSilent(bool aSilent);
            bool isSilent();

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

            QString m_strMainIcon;
            QString m_strKey;
            QString m_strVersionCode;
            QString m_strProductCode;
            QString m_strVersion;

            Sara::Product m_oProduct;
            Sara::ProductVersion m_oCurrentVersion;

            QList<Sara::Update> m_listUpdates;
            QList<Sara::Message> m_listMessages;

    };
}

#endif // CONFIG_H
