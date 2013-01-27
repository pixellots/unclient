#include <stdlib.h>
#include "config.h"
#include "settings.h"

using namespace Sara;

Config* Config::m_pInstance = NULL;

Config* Config::Instance()
{
   if(!m_pInstance)
       m_pInstance = new Config;

   return m_pInstance;
}

Config::Config()
{
    m_bSilent = FALSE;
}

Config::Config(Config const&)
{}

bool Config::isVersionMapped()
{
    Sara::Settings settings;

    if(!getVersionCode().isEmpty())
    {
        if(settings.isVersionMapped(getVersionCode()))
        {
            setProductCode(settings.getMappedProductCode());
            setVersion(settings.getMappedVersion());
            setVersionCode(settings.getMappedVersionCode());
            return TRUE;
        }
    }
    else
    {
        if(settings.isVersionMapped(getProductCode(), getVersion()))
        {
            setProductCode(settings.getMappedProductCode());
            setVersion(settings.getMappedVersion());
            setVersionCode(settings.getMappedVersionCode());
            return TRUE;
        }
    }
    return FALSE;
}

void Config::setKey(const QString& aKey)
{
    m_strKey = aKey;
}

QString Config::getKey() const
{
    return m_strKey;
}

void Config::setVersionCode(const QString& aVersionCode)
{
    m_strVersionCode = aVersionCode;
}

QString Config::getVersionCode() const
{
    return m_strVersionCode;
}

void Config::setProductCode(const QString& aProductCode)
{
    m_strProductCode = aProductCode;
}

QString Config::getProductCode() const
{
    return m_strProductCode;
}

void Config::setVersion(const QString& aVersion)
{
    m_strVersion = aVersion;
}

QString Config::getVersion() const
{
    return m_strVersion;
}

void Config::setSilent(bool aSilent)
{
    m_bSilent = aSilent;
}

bool Config::isSilent()
{
    return m_bSilent;
}

QString Config::getOS() const
{
#ifdef Q_OS_UNIX
    return "Linux";
#else
#ifdef Win32
    return "Windows";
#else
    return "Mac";
#endif
#endif
}

Sara::Product Config::product()
{
    return m_oProduct;
}

Sara::ProductVersion Config::version()
{
    return m_oCurrentVersion;
}

QList<Sara::Update> Config::updates()
{
    return m_listUpdates;
}

QList<Sara::Message> Config::messages()
{
    return m_listMessages;
}

void Config::addUpdate(const Sara::Update& aUpdate)
{
    m_listUpdates.append(aUpdate);
}

void Config::addMessage(const Sara::Message& aMessage)
{
    m_listMessages.append(aMessage);
}

void Config::setProduct(const Product& aProduct)
{
    m_oProduct = aProduct;
}

void Config::setVersion(const ProductVersion& aVersion)
{
    m_oCurrentVersion = aVersion;
}

void Config::clear()
{
    m_listUpdates.clear();
    m_listMessages.clear();
}
