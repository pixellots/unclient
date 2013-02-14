#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include "update.h"
#include "message.h"
#include "product.h"

#define SARA_COMPANY_STR        "Sara"
#define SARA_APPLICATION_STR    "Client"

namespace Sara
{
    class Settings : public QSettings
    {
        public:
            Settings();

        public:
            void setDownloadPath(const QString& aPath);
            QString getDownloadPath() const;

            QString uuid();

            void setUpdate(Sara::Update aUpdate, const QString& aLocalFile, int aResult);
            void setMessage(Sara::Message aMessage, bool aShown, bool aLoaded);
            void setMessage(Sara::Message aMessage, bool aShown);
            void setNewVersion(Sara::Product aProduct, Sara::ProductVersion aVersion);

            QString getProductCode();
            QString getProductVersion();
            QString getVersionCode();

            QString getMappedProductCode() const;
            QString getMappedVersionCode() const;
            QString getMappedVersion() const;

            bool messageShownAndLoaded(const QString& aMessageCode);

        private:
            bool isVersionMapped(const QString& aProductCode, const QString& aVersion);
            bool isVersionMapped(const QString& aVersionCode);


        private:
            QString m_strDownloadPath;

            QString m_strUUID;
            QString m_strUpdate;
            QString m_strMessage;
            QString m_strCurrentVersion;

            QString m_strMappedProductCode;
            QString m_strMappedVersionCode;
            QString m_strMappedVersion;


    };
}

#endif // SETTINGS_H
