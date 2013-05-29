#include <stdlib.h>
#include "config.h"

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
    m_bSilent = false;
    m_bSytemTray = false;
    m_bSingleMode = false;
}

Config::Config(Config const&)
{}

void Config::setKey(const QString& aKey)
{
    m_strKey = aKey;
}

QString Config::getKey() const
{
    return m_strKey;
}

void Config::setTestKey(const QString& aTestKey)
{
    m_strTestKey = aTestKey;
}

QString Config::getTestKey() const
{
    return m_strTestKey;
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

QString Config::mainIcon() const
{
    return m_strMainIcon;
}

void Config::setMainIcon(const QString& aIcon)
{
    m_strMainIcon = aIcon;
}

void Config::setLanguage(const QString& aLanguage)
{
    m_strLanguage = aLanguage;
}

QString Config::getLanguage() const
{
    return m_strLanguage;
}

void Config::clear()
{
    m_listUpdates.clear();
    m_listMessages.clear();
}

void Config::setSystemTray(bool aTray)
{
    m_bSytemTray = aTray;
}

bool Config::isSystemTray()
{
    return m_bSytemTray;
}

void Config::setSingleMode(bool aSingleMode)
{
    m_bSingleMode = aSingleMode;
}

bool Config::isSingleMode()
{
    return m_bSingleMode;
}

int Config::getUpdateInterval()
{
    return m_iUpdateInterval;
}

void Config::setUpdateInterval(int aInterval)
{
    m_iUpdateInterval = aInterval;
}

void Config::setHost(const QString& aHost)
{
    m_strHost = aHost;
}

QString Config::getHost() const
{
    return m_strHost;
}


