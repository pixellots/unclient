#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include "update.h"
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
            QString uuid();

            void setUpdate(Sara::Update aUpdate, const QString& aLocalFile, int aResult);
            void setNewVersion(Sara::Product aProduct, Sara::ProductVersion aVersion);

            bool isVersionMapped(const QString& aProductCode, const QString& aVersion);
            bool isVersionMapped(const QString& aVersionCode);

            QString getMappedProductCode() const;
            QString getMappedVersionCode() const;
            QString getMappedVersion() const;

        private:
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
