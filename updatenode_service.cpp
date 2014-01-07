/****************************************************************************
**
** Copyright (C) 2013 UpdatNode UG.
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
#include <QtDebug>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkProxyFactory>

#include "updatenode_service.h"
#include "config.h"
#include "settings.h"
#include "version.h"
#include "xmlparser.h"
#include "product.h"
#include "osdetection.h"
#include "logging.h"

using namespace UpdateNode;

/*!
\class UpdateNode::Service
\brief Main class for the communication between UpdateNode service and client
\n\n
This class connects your update/message definition on UpdateNode.com with your client.
*/

/*!
Constructs a Service object
*/
Service::Service(QObject* parent)
    : QObject(parent)
{
    m_pManager = NULL;
    m_pDownloader = NULL;
    m_iStatus = -1;

    // Use system proxy settings - if set
    QNetworkProxyFactory::setUseSystemConfiguration(true);
}

/*!
Destructs the Service object
*/
Service::~Service()
{
    m_pManager->deleteLater();
    m_pDownloader->deleteLater();
}

/*!
Checks for updates with parameters specified via UpdateNode::Config, or as registered \n
before for multi update usage
\sa Service::checkForUpdates(UpdateNode::Config* aConfig)
\sa Service::requestReceived
*/
bool Service::checkForUpdates()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    m_iStatus = -1;

    if(!m_pManager)
    {
        m_pManager = new QNetworkAccessManager(this);

        connect(m_pManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(requestReceived(QNetworkReply*)));
    }

    m_mapConfig.clear();

    if(config->isSingleMode())
        return checkForUpdates(config);
    else
    {
        bool result = true;
        for(int i = 0; i < config->configurations().size(); i++)
            result = result && checkForUpdates(config->configurations().at(i));
        return result;
    }

    return true;
}

/*!
Checks for updates with parameters specified by \aaConfig
\sa Service::requestReceived
*/
bool Service::checkForUpdates(UpdateNode::Config* aConfig)
{
    UpdateNode::Config* globalConfig = UpdateNode::Config::Instance();
    UpdateNode::Settings settings;

    QNetworkRequest request;
    QUrl url(UPDATENODE_SERVICE_URL);

    if(!globalConfig->getHost().isEmpty())
        url = url.fromUserInput(globalConfig->getHost());

    url.addQueryItem("key", globalConfig->getKey());

    if(!globalConfig->getTestKey().isEmpty())
        url.addQueryItem("test", globalConfig->getTestKey());

    url.addQueryItem("id", settings.uuid());
    url.addQueryItem("os", UpdateNode::OSDetection::getOS());
    url.addQueryItem("arch", UpdateNode::OSDetection::getArch());
    url.addQueryItem("lang", globalConfig->getLanguage());

    if(aConfig->getVersionCode().isEmpty())
    {
        url.addQueryItem("productCode", settings.getProductCode(aConfig));
        url.addQueryItem("productVersion", settings.getProductVersion(aConfig));
    }
    else
        url.addQueryItem("versionCode", settings.getVersionCode(aConfig));

    UpdateNode::Logging() << "REQUEST: " << url.toString();
    request.setUrl(url);
    request.setRawHeader("charset", "utf-8" );
    request.setRawHeader("User-Agent", QString("UpdateNodeClient %1 (%2)").arg(UPDATENODE_CLIENT_VERSION).arg(globalConfig->getOS()).toAscii());

    QNetworkReply* reply = m_pManager->get(request);

    m_mapConfig[reply] = aConfig;

    return true;
}

/*!
Slot called when the request has been returned. Emits done() for single app mode, or doneManager()\n
for multi app mode.
\note If the returned product definition contains an icon, the signal is emitted after the icon\n
has been downloaded
*/
void Service::requestReceived(QNetworkReply* reply)
{
    reply->deleteLater();

    UpdateNode::Config* config = m_mapConfig.take(reply);

    if(reply->error() == QNetworkReply::NoError)
    {
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            QString replyText = QString::fromUtf8(reply->readAll());

            UpdateNode::XmlParser* parser = new UpdateNode::XmlParser(this, config);
            parser->parse(replyText);
            m_strStatus = parser->getStatusString();
            m_iStatus = parser->getStatus();
            UpdateNode::Logging() << "RESULT: " << parser->getStatusString() << "(" << parser->getStatus() << ")";
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            newUrl = reply->url().resolved(newUrl);

            QNetworkAccessManager *manager = reply->manager();
            QNetworkRequest redirection(newUrl);
            manager->get(redirection);

            return;
        }
    }
    else
    {
        // Error
        UpdateNode::Logging() << "ERROR: " << reply->errorString();
    }

    if(!config->product().getIconUrl().isEmpty())
    {
        if(!m_pDownloader)
            m_pDownloader = new UpdateNode::Downloader();

        if(UpdateNode::Config::Instance()->isSingleMode())
            connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), this, SIGNAL(done()));
        else if(m_mapConfig.size()==0)
            connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), this, SIGNAL(doneManager()));

        m_pDownloader->doDownload(config->product().getIconUrl(), UpdateNode::Update());
    }
    else
    {
        if(UpdateNode::Config::Instance()->isSingleMode())
            emit done();
        else if(m_mapConfig.size()==0)
            emit doneManager();
    }
}

/*!
Builds the return code based on the results from all checked products
\sa Service::returnCode
*/
int Service::returnCodeManager()
{
    QString result;
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    int update_cnt = 0;
    int message_cnt = 0;
    for(int i = 0; i < config->configurations().size(); i++)
    {
        update_cnt += config->configurations().at(i)->updates().size();
        message_cnt += config->configurations().at(i)->messages().size();
    }
    return returnCode(update_cnt, message_cnt);
}


/*!
Builds the return code based on the results from a specified Config \aconfig
\sa Service::returnCode
*/
int Service::returnCode(UpdateNode::Config* config /* = NULL */)
{
    if(!config)
        config = UpdateNode::Config::Instance();

    int update_cnt = config->updates().size();
    int message_cnt = config->messages().size();

    return returnCode(update_cnt, message_cnt);
}

/*!
Builds the return code based on update counter \aaUpdateCount and message \n
counter \aaMessageCount
\sa Service::returnCode
*/
int Service::returnCode(int aUpdateCount, int aMessageCount)
{
    Service::Status status;

    if(aUpdateCount == 0 && aMessageCount == 0)
        status = Service::NOTHING; // nothing
    else if(aUpdateCount > 0 && aMessageCount == 0)
        status = Service::UPDATE; // update
    else if(aUpdateCount == 0 && aMessageCount >= 1)
        status = Service::MESSAGE; // message
    else
        status = Service::UPDATE_MESSAGE; // update & message

    if(UpdateNode::Config::Instance()->isSystemTray())
    {
        switch(status)
        {
            case Service::MESSAGE:
                return Service::NOTHING;
            default:
                return status;
       }
    }

    return status;
}

/*!
Returns the notification text based on return code from Service::returnCode
*/
QString Service::notificationText(UpdateNode::Config* config /* = NULL */)
{
    QString text;
    switch(returnCode(config))
    {
        case Service::NOTHING:
            text = QObject::tr("There are no new updates & messages available");
            break;
        case Service::UPDATE:
            text = QObject::tr("There are new updates available");
            break;
        case Service::MESSAGE:
            text = QObject::tr("There are new messages available");
            break;
        case Service::UPDATE_MESSAGE:
            text = QObject::tr("There are updates and messages available");
            break;
        default:
            text = QObject::tr("Undefined state");
            break;
    }
    return text;
}

/*!
Returns the notification text based on return code from Service::returnCodeManager
*/
QString Service::notificationTextManager()
{
    QString text;
    switch(returnCodeManager())
    {
        case Service::NOTHING:
            text = QObject::tr("There are no new updates & messages available");
            break;
        case Service::UPDATE:
            text = QObject::tr("There are new updates available");
            break;
        case Service::MESSAGE:
            text = QObject::tr("There are new messages available");
            break;
        case Service::UPDATE_MESSAGE:
            text = QObject::tr("There are updates and messages available");
            break;
        default:
            text = QObject::tr("Undefined state");
            break;
    }
    return text;
}

/*!
Returns the service status error code
*/
int Service::status()
{
    return m_iStatus;
}

/*!
Returns the service status error string
*/
QString Service::statusText() const
{
    return m_strStatus;
}

