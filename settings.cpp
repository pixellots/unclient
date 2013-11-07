#include <stdlib.h>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QUuid>
#include <QDir>
#include "settings.h"
#include "config.h"

using namespace UpdateNode;

Settings::Settings()
    : QSettings(UPDATENODE_COMPANY_STR, UPDATENODE_APPLICATION_STR, 0)
{

    QString id = UpdateNode::Config::Instance()->getKey();

    m_strUUID = QString("uuid");

    id += "/";

    m_strDownloadPath   = id + QString("DownloadPath");
    m_strClientPath     = id + QString("ClientPath/");
    m_strUpdate         = id + QString("Update/");
    m_strMessage        = id + QString("Message/");
    m_strCurrentVersion = id + QString("CurrentVersion/");
    m_strRegistrations  = id + QString("Registered/");
}

void Settings::setDownloadPath(const QString& aPath)
{
    this->setValue(m_strDownloadPath, aPath);
}

QString Settings::getDownloadPath() const
{
    return this->value(m_strDownloadPath, QDir::tempPath()).toString();
}

QString Settings::uuid()
{
    QString id = this->value(m_strUUID).toString();

    if(id==QString::null)
    {
        id = QUuid::createUuid().toString();
        this->setValue(m_strUUID, id);
    }

    return id;
}

bool Settings::registerVersion()
{
    QString id;
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    if(!config->getVersion().isEmpty()
            && !config->getProductCode().isEmpty())
        id = m_strRegistrations + config->getProductCode() + "/";
    else
        return false;

    this->setValue( id + "Version" , config->getVersion());

    return true;
}

bool Settings::unRegisterVersion()
{
    QString id;
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    if(!config->getVersion().isEmpty()
            && !config->getProductCode().isEmpty())
        id = m_strRegistrations + config->getProductCode();
    else
        return false;

    this->remove(id);

    return true;
}

bool Settings::getRegisteredVersion()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    QStringList codes;

    beginGroup(m_strRegistrations);
    codes = childGroups();
    endGroup();

    for(int i = 0; i < codes.size(); i++)
    {
        QString id = m_strRegistrations + codes.at(i) + "/";
        if(!this->value(id + "Version", "").toString().isEmpty())
        {
            UpdateNode::Config* poolConfig = new UpdateNode::Config();
            poolConfig->setProductCode(codes.at(i));
            poolConfig->setVersion(this->value(id + "Version", "").toString());

            config->addConfiguration(poolConfig);
        }
    }
    return true;
}

void Settings::setUpdate(UpdateNode::Update aUpdate, const QString& aLocalFile, int aResult)
{
    QString id = m_strUpdate + aUpdate.getCode() + "/";

    this->setValue( id + "Result" , aResult);
    this->setValue( id + "File" , aLocalFile);
    this->setValue( id + "Type" , aUpdate.getType());
}

void  Settings::setMessage(UpdateNode::Message aMessage, bool aShown, bool aLoaded)
{
    QString id = m_strMessage + aMessage.getCode() + "/";

    this->setValue( id + "Shown" , aShown);
    this->setValue( id + "Loaded" , aLoaded);
}

void  Settings::setMessage(UpdateNode::Message aMessage, bool aShown)
{
    QString id = m_strMessage + aMessage.getCode() + "/";

    this->setValue( id + "Shown" , aShown);
}

bool Settings::messageShownAndLoaded(const QString& aMessageCode)
{
    QString id = m_strMessage + aMessageCode + "/";

    return this->value( id + "Shown" , false).toBool() && this->value( id + "Loaded" , false).toBool();
}

void Settings::setNewVersion(UpdateNode::Config* config, UpdateNode::Product aProduct, UpdateNode::ProductVersion aVersion)
{
    QString id = m_strCurrentVersion + aProduct.getCode() + "/";

    this->setValue( id + "Name", aProduct.getName());
    this->setValue( id + "Old/Version", config->getVersion());
    this->setValue( id + "Old/Code", config->getVersionCode());

    this->setValue( id + "Version/Version", aVersion.getVersion());
    this->setValue( id + "Version/Code", aVersion.getCode());
    this->setValue( id + "Version/Name", aVersion.getName());
}

bool Settings::isVersionMapped(const QString& aProductCode, const QString& aVersion)
{
    QString id = m_strCurrentVersion + aProductCode + "/";

    if(!this->value(id + "Name").isNull()
        && this->value(id + "Version/Version").toString() != aVersion)
    {
        // Currently single products only are supported
        m_strMappedProductCode = aProductCode;

        m_strMappedVersion = this->value(id + "Version/Version").toString();
        m_strMappedVersionCode = this->value(id + "Version/Code").toString();
        return true;
    }
    else
        return false;
}

bool Settings::isVersionMapped(const QString& aVersionCode)
{
    QStringList productCodes;

    beginGroup(m_strCurrentVersion);
    productCodes = childGroups();
    endGroup();

    for(int i = 0; i < productCodes.size(); i++)
    {
        QString id = m_strCurrentVersion + productCodes.at(i) + "/";
        if(aVersionCode == this->value(id + "Old/Code").toString())
        {
            m_strMappedProductCode = productCodes.at(i);

            m_strMappedVersion = this->value(id + "Version/Version").toString();
            m_strMappedVersionCode = this->value(id + "Version/Code").toString();
            return true;
        }
    }
    return false;
}

QString Settings::getMappedProductCode() const
{
    return m_strMappedProductCode;
}

QString Settings::getMappedVersionCode() const
{
    return m_strMappedVersionCode;
}

QString Settings::getMappedVersion() const
{
    return m_strMappedVersion;
}

QString Settings::getProductCode(UpdateNode::Config* aConfig /* = null */)
{
    UpdateNode::Config* config;

    if(aConfig)
        config = aConfig;
    else
        config = UpdateNode::Config::Instance();

    if(isVersionMapped(config->getProductCode(), config->getVersion()))
        return getMappedProductCode();
    else
        return config->getProductCode();
}

QString Settings::getProductVersion(UpdateNode::Config* aConfig /* = null */)
{
    UpdateNode::Config* config;

    if(aConfig)
        config = aConfig;
    else
        config = UpdateNode::Config::Instance();

    if(isVersionMapped(config->getProductCode(), config->getVersion()))
        return getMappedVersion();
    else
        return config->getVersion();
}

QString Settings::getVersionCode(UpdateNode::Config* aConfig /* = null */)
{
    UpdateNode::Config* config;

    if(aConfig)
        config = aConfig;
    else
        config = UpdateNode::Config::Instance();

    if(isVersionMapped(config->getVersionCode()))
        return getMappedVersionCode();
    else
        return config->getVersionCode();
}

void Settings::setCachedFile(const QString& aCode, const QString& aFilename)
{
    if(aCode.isEmpty())
        return;

    QString id = m_strUpdate + aCode + "/";

    this->setValue( id + "File" , aFilename);
}

QString Settings::getCachedFile(const QString& aCode)
{
    QString id = m_strUpdate + aCode + "/";

    return this->value( id + "File").toString();
}

void Settings::setCurrentClientDir(const QString& aClientDir)
{
    this->setValue( m_strClientPath + Config::Instance()->getKey() , aClientDir);
}

QString Settings::getCurrentClientDir()
{
    return this->value( m_strClientPath + Config::Instance()->getKey()).toString();
}
