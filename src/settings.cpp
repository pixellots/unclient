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

#include <stdlib.h>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QUuid>
#include <QDir>
#include "settings.h"
#include "config.h"

using namespace UpdateNode;

/*!
\class UpdateNode::Settings
\brief Main class for storing and reading setting informations
\n\n
Using this class, you can access all your settings data stored while executing the client.
*/

/*!
Constructs an Settings object, referening to company "UpdateNode" and "Client" as \n
application name.
*/
Settings::Settings()
    : QSettings(UPDATENODE_COMPANY_STR, UPDATENODE_APPLICATION_STR, 0)
{

    QString id = UpdateNode::Config::Instance()->getKeyHashed();

    m_strUUID = QString("uuid");

    id += "/";

    m_strDownloadPath   = id + QString("DownloadPath");
    m_strClientPath     = id + QString("ClientPath/");
    m_strUpdate         = id + QString("Update/");
    m_strMessage        = id + QString("Message/");
    m_strCurrentVersion = id + QString("CurrentVersion/");
    m_strRegistrations  = id + QString("Registered/");
}

/*!
Sets the download path
\sa Setttings::getDownloadPath
*/
void Settings::setDownloadPath(const QString& aPath)
{
    this->setValue(m_strDownloadPath, aPath);
}

/*!
Returns the download path, or system's temp path if no download path is specified
\sa Setttings::setDownloadPath
*/
QString Settings::getDownloadPath() const
{
    return this->value(m_strDownloadPath, QDir::tempPath()).toString();
}

/*!
Returns the current UUID. When so UUID is set, a new will be generated.
\n The new generated UUID is stored for any further usage.
*/
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

/*!
Registering the current used product version, as specified in UpdateNode::Config \n
Returns false if Config is not set proper
\sa Settings::unRegisterVersion
*/
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

/*!
Unregistering the current used product version, as specified in UpdateNode::Config \n
Returns false if Config is not set proper
\sa Settings::registerVersion
*/
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

/*!
Builds the configuration list based on previous registered versions
\sa Settings::registerVersion
\sa Settings::unRegisterVersion
\sa Config::configurations
*/
void Settings::getRegisteredVersion()
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
}

/*!
Stores informations about the specified update \aaUpdate, it's local file destination and \n
the returned exit code
*/
void Settings::setUpdate(UpdateNode::Update aUpdate, const QString& aLocalFile, int aResult)
{
    QString id = m_strUpdate + aUpdate.getCode() + "/";

    this->setValue( id + "Result" , aResult);
    this->setValue( id + "File" , aLocalFile);
    this->setValue( id + "Type" , aUpdate.getType());
}

/*!
Stores informations about the specified message \aaMessage and if it has been shown and loaded
*/
void  Settings::setMessage(UpdateNode::Message aMessage, bool aShown, bool aLoaded)
{
    QString id = m_strMessage + aMessage.getCode() + "/";

    this->setValue( id + "Shown" , aShown);
    this->setValue( id + "Loaded" , aLoaded);
}

/*!
Stores informations about the specified message \aaMessage and if it has been shown
*/
void  Settings::setMessage(UpdateNode::Message aMessage, bool aShown)
{
    QString id = m_strMessage + aMessage.getCode() + "/";

    this->setValue( id + "Shown" , aShown);
}

/*!
Checked whether a message specified by \aaMessageCode has been loaded and shown
*/
bool Settings::messageShownAndLoaded(const QString& aMessageCode)
{
    QString id = m_strMessage + aMessageCode + "/";

    return this->value( id + "Shown" , false).toBool() && this->value( id + "Loaded" , false).toBool();
}

/*!
Maps the current product version specified in \aconfig to information from \n
parameters \aaProduct and \aaVersion
*/
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

/*!
Checkes whether a product code and product version is mapped or not
*/
bool Settings::isVersionMapped(const QString& aProductCode, const QString& aVersion)
{
    QString id = m_strCurrentVersion + aProductCode + "/";

    if(!this->value(id + "Name").isNull()
        && this->value(id + "Version/Version").toString() != aVersion)
    {
        m_strMappedProductCode = aProductCode;

        m_strMappedVersion = this->value(id + "Version/Version").toString();
        m_strMappedVersionCode = this->value(id + "Version/Code").toString();
        return true;
    }
    else
        return false;
}

/*!
Checkes whether a version code is mapped or not
*/
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

/*!
Returns the mapped product code
\note Settings::isVersionMapped needs to be called before
*/
QString Settings::getMappedProductCode() const
{
    return m_strMappedProductCode;
}

/*!
Returns the mapped version code
\note Settings::isVersionMapped needs to be called before
*/
QString Settings::getMappedVersionCode() const
{
    return m_strMappedVersionCode;
}

/*!
Returns the mapped product version
\note Settings::isVersionMapped needs to be called before
*/
QString Settings::getMappedVersion() const
{
    return m_strMappedVersion;
}

/*!
Returns the current product code for a specific UpdateNode::Config \aaConfig object
*/
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

/*!
Returns the current product version for a specific UpdateNode::Config \aaConfig object
*/
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

/*!
Returns the current version code for a specific UpdateNode::Config \aaConfig object
*/
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

/*!
Stores a cached file for a given update code \aaCode and it's \aaFilename
\sa Settings::getCachedFile
*/
void Settings::setCachedFile(const QString& aCode, const QString& aFilename)
{
    if(aCode.isEmpty())
        return;

    QString id = m_strUpdate + aCode + "/";

    this->setValue( id + "File" , aFilename);
}

/*!
Returns the cached file for a given update code \aaCode
\sa Settings::setCachedFile
*/
QString Settings::getCachedFile(const QString& aCode)
{
    QString id = m_strUpdate + aCode + "/";

    return this->value( id + "File").toString();
}

/*!
Sets the current client dir, specified by \aaClientDir
\sa Settings::getCurrentClientDir
*/
void Settings::setCurrentClientDir(const QString& aClientDir)
{
    this->setValue( m_strClientPath + Config::Instance()->getKeyHashed() , aClientDir);
}

/*!
Returns the current client dir
\sa Settings::setCurrentClientDir
*/
QString Settings::getCurrentClientDir()
{
    return this->value( m_strClientPath + Config::Instance()->getKeyHashed()).toString();
}

void Settings::setIgnoreUpdate(const QString& aUpdateCode, bool aIgnore)
{
    QString id = m_strUpdate + aUpdateCode + "/";

    this->setValue( id + "Ignore" , aIgnore);
}

bool Settings::isUpdateIgnored(const QString& aUpdateCode)
{
    QString id = m_strUpdate + aUpdateCode + "/";

    return this->value( id + "Ignore" , false).toBool();
}
