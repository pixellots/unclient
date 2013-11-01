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
            bool getRegisteredVersion();

            void setUpdate(UpdateNode::Update aUpdate, const QString& aLocalFile, int aResult);
            void setMessage(UpdateNode::Message aMessage, bool aShown, bool aLoaded);
            void setMessage(UpdateNode::Message aMessage, bool aShown);
            void setNewVersion(UpdateNode::Config* config, UpdateNode::Product aProduct, UpdateNode::ProductVersion aVersion);

            QString getProductCode(UpdateNode::Config* aConfig = NULL);
            QString getProductVersion(UpdateNode::Config* aConfig = NULL);
            QString getVersionCode(UpdateNode::Config* aConfig = NULL);

            QString getMappedProductCode() const;
            QString getMappedVersionCode() const;
            QString getMappedVersion() const;

            bool messageShownAndLoaded(const QString& aMessageCode);

            void setCachedFile(const QString& aCode, const QString& aFilename);
            QString getCachedFile(const QString& aCode);

            void setCurrentClientDir(const QString& aClientDir);
            QString getCurrentClientDir();

        private:
            bool isVersionMapped(const QString& aProductCode, const QString& aVersion);
            bool isVersionMapped(const QString& aVersionCode);


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
