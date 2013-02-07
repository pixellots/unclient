#include <stdlib.h>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QUuid>
#include "settings.h"
#include "config.h"

using namespace Sara;

Settings::Settings()
    : QSettings(SARA_COMPANY_STR, SARA_APPLICATION_STR, 0)
{
    m_strUUID = QString("uuid");
    m_strUpdate = QString("Update/");
    m_strMessage = QString("Message/");
    m_strCurrentVersion = QString("CurrentVersion/");
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

void Settings::setUpdate(Sara::Update aUpdate, const QString& aLocalFile, int aResult)
{
    QString id = m_strUpdate + aUpdate.getCode() + "/";

    this->setValue( id + "Result" , aResult);
    this->setValue( id + "File" , aLocalFile);
    this->setValue( id + "Type" , aUpdate.getType());
}

void  Settings::setMessage(Sara::Message aMessage, bool aShown, bool aLoaded)
{
    QString id = m_strMessage + aMessage.getCode() + "/";

    this->setValue( id + "Shown" , aShown);
    this->setValue( id + "Loaded" , aLoaded);
}

void  Settings::setMessage(Sara::Message aMessage, bool aShown)
{
    QString id = m_strMessage + aMessage.getCode() + "/";

    this->setValue( id + "Shown" , aShown);
}

bool Settings::messageShownAndLoaded(const QString& aMessageCode)
{
    QString id = m_strMessage + aMessageCode + "/";

    return this->value( id + "Shown" , false).toBool() && this->value( id + "Loaded" , false).toBool();
}

void Settings::setNewVersion(Sara::Product aProduct, Sara::ProductVersion aVersion)
{
    QString id = m_strCurrentVersion + aProduct.getCode() + "/";

    this->setValue( id + "Name", aProduct.getName());
    this->setValue( id + "Old/Version", Sara::Config::Instance()->getVersion());
    this->setValue( id + "Old/Code", Sara::Config::Instance()->getVersionCode());

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

QString Settings::getProductCode()
{
    if(isVersionMapped(Sara::Config::Instance()->getProductCode(), Sara::Config::Instance()->getVersion()))
        return getMappedProductCode();
    else
        return Sara::Config::Instance()->getProductCode();
}

QString Settings::getProductVersion()
{
    if(isVersionMapped(Sara::Config::Instance()->getProductCode(), Sara::Config::Instance()->getVersion()))
        return getMappedVersion();
    else
        return Sara::Config::Instance()->getVersion();
}

QString Settings::getVersionCode()
{
    if(isVersionMapped(Sara::Config::Instance()->getVersionCode()))
        return getMappedVersionCode();
    else
        return Sara::Config::Instance()->getVersionCode();
}
