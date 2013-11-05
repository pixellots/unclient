#include <QLocale>
#include <stdlib.h>
#include "config.h"

using namespace UpdateNode;

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
    m_bRelaunch = false;
}

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

UpdateNode::Product Config::product()
{
    return m_oProduct;
}

UpdateNode::ProductVersion Config::version()
{
    return m_oCurrentVersion;
}

QList<UpdateNode::Update> Config::updates()
{
    return m_listUpdates;
}

QList<UpdateNode::Message> Config::messages()
{
    return m_listMessages;
}

void Config::addUpdate(const UpdateNode::Update& aUpdate)
{
    m_listUpdates.append(aUpdate);
}

void Config::addMessage(const UpdateNode::Message& aMessage)
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
    return m_strLanguage.isEmpty() ? QLocale::system().name() : m_strLanguage;
}

void Config::clear()
{
    m_listUpdates.clear();
    m_listMessages.clear();
    m_listConfigs.clear();
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

void Config::setHost(const QString& aHost)
{
    m_strHost = aHost;
}

QString Config::getHost() const
{
    return m_strHost;
}

void Config::addConfiguration(UpdateNode::Config* aConfig)
{
    m_listConfigs.append(aConfig);
}

QList<UpdateNode::Config*> Config::configurations()
{
    return m_listConfigs;
}

void Config::clearConfigurations()
{
    m_listConfigs.clear();
}

void Config::setLogging(const QString &aFileName)
{
    m_strLogging = aFileName;
}

bool Config::isLoggingEnabled()
{
    return !m_strLogging.isEmpty();
}

QString Config::getLoggingFile()
{
    return m_strLogging;
}

void Config::setExec(const QString& aFileName)
{
    m_strExec = aFileName;
}

QString Config::getExec()
{
    return m_strExec;
}

void Config::setRelaunch(bool aEnable)
{
    m_bRelaunch = aEnable;
}

bool Config::isRelaunch()
{
    return m_bRelaunch;
}




