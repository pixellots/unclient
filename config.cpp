/****************************************************************************
**
** Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
** Contact: code@updatenode.com
**
** This file is part of the UpdateNode Client.
**
** Commercial License Usage
** Licensees holding valid commercial UpdateNode license may use this file
** under the terms of the the Apache License, Version 2.0
** Full license description file: LICENSE.COM
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation. Please review the following information to ensure the
** GNU General Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
** Full license description file: LICENSE.GPL
**
****************************************************************************/

#include <QLocale>
#include <QCryptographicHash>

#include <stdlib.h>
#include "config.h"

using namespace UpdateNode;

/*!
\class UpdateNode::Config
\brief Main config class which holds all informations about the current requested
product, its version and all information required to retreive and display updates
and messages
*/

/*!
Global instance of the config data - used for program confiuration and single product
updates and messages
*/
Config* Config::m_pInstance = NULL;

/*!
Retrieves the global Config class instance as an pointer. If no instance is present,
a new one will be created in this method
*/
Config* Config::Instance()
{
   if(!m_pInstance)
       m_pInstance = new Config;

   return m_pInstance;
}

/*!
Constructs a Config object
*/
Config::Config()
{
    m_bSilent = false;
    m_bSytemTray = false;
    m_bSingleMode = false;
    m_bRelaunch = false;
}

/*!
Sets the API key which has been assigned in UpdateNode.com
\sa Config::getKey
*/
void Config::setKey(const QString& aKey)
{
    m_strKey = aKey;
}

/*!
Returns the API key which has been assigned in UpdateNode.com
\sa Config::setKey
*/
QString Config::getKey() const
{
    return m_strKey;
}

/*!
Returns a MD5 hashed representation of the API key previously set by Config::setkey
\sa Config::setKey
*/
QString Config::getKeyHashed() const
{
    return QString(QCryptographicHash::hash(getKey().toAscii(), QCryptographicHash::Md5).toHex());
}

/*!
Sets the Test key which has been assigned in UpdateNode.com
\sa Config::getTestKey
*/
void Config::setTestKey(const QString& aTestKey)
{
    m_strTestKey = aTestKey;
}

/*!
Returns the Test key which has been assigned in UpdateNode.com
\sa Config::setTestKey
*/
QString Config::getTestKey() const
{
    return m_strTestKey;
}

/*!
Sets the version code
\sa Config::getVersionCode
*/
void Config::setVersionCode(const QString& aVersionCode)
{
    m_strVersionCode = aVersionCode;
}

/*!
Returns the version code
\sa Config::setVersionCode
*/
QString Config::getVersionCode() const
{
    return m_strVersionCode;
}

/*!
Sets the product code
\sa Config::getProductCode
*/
void Config::setProductCode(const QString& aProductCode)
{
    m_strProductCode = aProductCode;
}

/*!
Returns the product code
\sa Config::setProductCode
*/
QString Config::getProductCode() const
{
    return m_strProductCode;
}

/*!
Sets the product's version as a string
\sa Config::getVersion
*/
void Config::setVersion(const QString& aVersion)
{
    m_strVersion = aVersion;
}

/*!
Returns the product's version as a string
\sa Config::setVersion
*/
QString Config::getVersion() const
{
    return m_strVersion;
}

/*!
Sets the silent mode
\sa Config::isSilent
*/
void Config::setSilent(bool aSilent)
{
    m_bSilent = aSilent;
}

/*!
Returns true if silent mode has been enabled
\sa Config::setSilent
*/
bool Config::isSilent()
{
    return m_bSilent;
}

/*!
Returns the current OS as String
\note Possible values are: Windows, Linux, or Mac
*/
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

/*!
Returns the Product object
\sa Config::setProduct
*/
UpdateNode::Product Config::product()
{
    return m_oProduct;
}

/*!
Returns the ProductVersion object
\sa Config::setVersion
*/
UpdateNode::ProductVersion Config::version()
{
    return m_oCurrentVersion;
}

/*!
Returns the update object's in a QList
\sa Config::addUpdate
*/
QList<UpdateNode::Update> Config::updates()
{
    return m_listUpdates;
}

/*!
Returns the message object's in a QList
\sa Config::addMessage
*/
QList<UpdateNode::Message> Config::messages()
{
    return m_listMessages;
}

/*!
Adds a new update into the updates list
\sa Config::updates
*/
void Config::addUpdate(const UpdateNode::Update& aUpdate)
{
    m_listUpdates.append(aUpdate);
}

/*!
Adds a new message into the message list
\sa Config::messages
*/
void Config::addMessage(const UpdateNode::Message& aMessage)
{
    m_listMessages.append(aMessage);
}

/*!
Sets the Product object
\sa Config::product
*/
void Config::setProduct(const Product& aProduct)
{
    m_oProduct = aProduct;
}

/*!
Sets the ProductVersion object
\sa Config::version
*/
void Config::setVersion(const ProductVersion& aVersion)
{
    m_oCurrentVersion = aVersion;
}

/*!
Returns the main icon
\sa Config::setMainIcon
*/
QString Config::mainIcon() const
{
    return m_strMainIcon;
}

/*!
Sets the main icon
\sa Config::mainIcon
*/
void Config::setMainIcon(const QString& aIcon)
{
    m_strMainIcon = aIcon;
}

/*!
Sets the langugage. Language needs to be at least two characted language code.
\sa Config::getLanguage
*/
void Config::setLanguage(const QString& aLanguage)
{
    m_strLanguage = aLanguage;
}

/*!
Returns the set langugage, or the systems language if Config::setLanguage was not called before.
\sa Config::setLanguage
*/
QString Config::getLanguage() const
{
    return m_strLanguage.isEmpty() ? QLocale::system().name() : m_strLanguage;
}

/*!
Clears the update, message and configurations list
*/
void Config::clear()
{
    m_listUpdates.clear();
    m_listMessages.clear();
    m_listConfigs.clear();
}

/*!
Enables systems tray icon
\sa Config::isSystemTray
*/
void Config::setSystemTray(bool aTray)
{
    m_bSytemTray = aTray;
}

/*!
Returns true if system tray has been activated
\sa Config::setSystemTray
*/
bool Config::isSystemTray()
{
    return m_bSytemTray;
}

/*!
Enabled the single mode. The single mode indicates that the update manager should be only
operating on on product.
\sa Config::isSingleMode
*/
void Config::setSingleMode(bool aSingleMode)
{
    m_bSingleMode = aSingleMode;
}

/*!
Returns true if singel mode has been activated
\sa Config::setSingleMode
*/
bool Config::isSingleMode()
{
    return m_bSingleMode;
}

/*!
Sets the UpdateNode host
\note this method is called by UpdateNode team internal to test new features in an separated environment
\sa Config::getHost
*/
void Config::setHost(const QString& aHost)
{
    m_strHost = aHost;
}

/*!
Returns the host
\sa Config::setHost
*/
QString Config::getHost() const
{
    return m_strHost;
}

/*!
Adds a configuration to the list of configurations. This is used for the multi-products mode
\sa Config::configurations
*/
void Config::addConfiguration(UpdateNode::Config* aConfig)
{
    m_listConfigs.append(aConfig);
}

/*!
Sets the langugage. Language needs to be at least two characted language code.
\sa Config::addConfiguration
*/
QList<UpdateNode::Config*> Config::configurations()
{
    return m_listConfigs;
}

/*!
Clears the list of configurations
\sa Config::clear
*/
void Config::clearConfigurations()
{
    m_listConfigs.clear();
}

/*!
Sets the logging file name
\sa Config::isLoggingEnabled
\sa Config::getLoggingFile
*/
void Config::setLogging(const QString &aFileName)
{
    m_strLogging = aFileName;
}

/*!
Checks if logging has been enabled or not. Logging can be disabled calling
Config::setLogging with an empty string as parameter
\sa Config::getLoggingFile
\sa Config::setLoggingFile
*/
bool Config::isLoggingEnabled()
{
    return !m_strLogging.isEmpty();
}

/*!
Returns the logging file name
\sa Config::isLoggingEnabled
\sa Config::setLoggingFile
*/
QString Config::getLoggingFile()
{
    return m_strLogging;
}

/*!
Sets executable and its parameters which should be executed once check/installation
has been performed
\sa Config::getExec
*/
void Config::setExec(const QString& aFileName)
{
    m_strExec = aFileName;
}

/*!
Sets the executable
\sa Config::setExec
*/
QString Config::getExec()
{
    return m_strExec;
}

/*!
Using the setRelaunch method, you are able to re-spawn the current unclient
in a save evniroment. Means, to some place from which you can overwrite the original binary.
This mode is mostly useful when you are running on Windows.
\sa Config::isRelaunch
*/
void Config::setRelaunch(bool aEnable)
{
    m_bRelaunch = aEnable;
}

/*!
Returns true if relaunch is enabled
\sa Config::setRelaunch
*/
bool Config::isRelaunch()
{
    return m_bRelaunch;
}

/*!
Sets the splash sceen.
\sa Config::getSplashScreen
*/
void Config::setSplashScreen(const QString& aFileName)
{
    m_strSplashImage = aFileName;
}

/*!
Returns the splash screen
\sa Config::setSplashScreen
*/
QString Config::getSplashScreen()
{
    return m_strSplashImage;
}





