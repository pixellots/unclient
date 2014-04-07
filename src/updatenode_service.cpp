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
#include <QtDebug>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkProxyFactory>
#include <QSslConfiguration>

#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

#include "updatenode_service.h"
#include "config.h"
#include "settings.h"
#include "version.h"
#include "xmlparser.h"
#include "product.h"
#include "osdetection.h"
#include "logging.h"
#include "limittimer.h"

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

    connect(this, SIGNAL(done()), UpdateNode::LimitTimer::Instance(), SLOT(stop()));
    connect(this, SIGNAL(doneManager()), UpdateNode::LimitTimer::Instance(), SLOT(stop()));
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
        connect(m_pManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(onSslError(QNetworkReply*,QList<QSslError>)));
    }

    m_mapConfig.clear();

    if(config->isSingleMode())
        return checkForUpdates(config);
    else
    {
        bool result = true;
        m_listConfigs =  config->configurations();
        while(!m_listConfigs.isEmpty())
            result = result && checkForUpdates(m_listConfigs.takeFirst());

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
    {
        UpdateNode::Logging() << "WARNING: Running in HTTP mode";
        url = url.fromUserInput(globalConfig->getHost());
    }

#if QT_VERSION >= 0x050000
    QUrlQuery url_query;
    url_query.addQueryItem("key", globalConfig->getKey());

    if(!globalConfig->getTestKey().isEmpty())
        url_query.addQueryItem("test", globalConfig->getTestKey());

    url_query.addQueryItem("id", settings.uuid());
    url_query.addQueryItem("os", UpdateNode::OSDetection::getOS());
    url_query.addQueryItem("arch", UpdateNode::OSDetection::getArch());
    url_query.addQueryItem("lang", globalConfig->getLanguage());

    if(aConfig->getVersionCode().isEmpty())
    {
        url_query.addQueryItem("productCode", settings.getProductCode(aConfig));
        url_query.addQueryItem("productVersion", settings.getProductVersion(aConfig));
    }
    else
        url_query.addQueryItem("versionCode", settings.getVersionCode(aConfig));

    url.setQuery(url_query);
#else
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

#endif
    request.setUrl(url);

#ifdef DEBUG
    UpdateNode::Logging() << "REQUEST: " << url.toString();
#endif

    request.setRawHeader("charset", "utf-8" );
    request.setRawHeader("User-Agent", QString("UpdateNode Client %1.%2.%3 (%4)").arg(APP_VERSION_HIGH).arg(APP_VERSION_LOW).arg(APP_VERSION_REV).arg(globalConfig->getOS()).toLatin1());

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

    UpdateNode::Config* config = m_mapConfig[reply];

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
            UpdateNode::Logging() << "UpdateNode RESULT: " << parser->getStatusString() << "(" << parser->getStatus() << ")";
#ifndef UNITTEST
            if(parser->getStatus()!=0)
            {
                qApp->exit(UPDATENODE_PROCERROR_SERVICE_ERROR);
                return;
            }
#endif
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Error
            UpdateNode::Logging() << "ERROR: Redirection not supported";
            return;
        }
    }
    else
    {
        // Error
        UpdateNode::Logging() << "ERROR: " << reply->errorString();
    }

    m_mapConfig.remove(reply);

    if(reply->error() == QNetworkReply::NoError && !config->product().getIconUrl().isEmpty())
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
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    int update_cnt = 0;
    int message_cnt = 0;
    for(int i = 0; i < config->configurations().size(); i++)
    {
        foreach(UpdateNode::Update update, config->configurations().at(i)->updates())
            if(!UpdateNode::Settings().isUpdateIgnored(update.getCode()))
                update_cnt++;

        /* currently not supported defect #1
         *
        foreach(UpdateNode::Message message, config->configurations().at(i)->messages())
            if(!UpdateNode::Settings().messageShownAndLoaded(message.getCode()))
                message_cnt++;
         */
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

    int update_cnt = 0;
    foreach(UpdateNode::Update update, config->updates())
        if(!UpdateNode::Settings().isUpdateIgnored(update.getCode()))
            update_cnt++;

    int message_cnt = 0;
    foreach(UpdateNode::Message message, config->messages())
        if(!UpdateNode::Settings().messageShownAndLoaded(message.getCode()))
            message_cnt++;
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
        case Service::MESSAGE:
            text = QObject::tr("There are no new updates & messages available");
            break;
        case Service::UPDATE:
        case Service::UPDATE_MESSAGE:
            text = QObject::tr("There are new updates available");
            break;
        default:
            text = QObject::tr("Undefined state");
            break;
    }
    return text;
}

/*!
Slot called on SSL errors
*/
void Service::onSslError(QNetworkReply *reply, const QList<QSslError>& errors)
{
    QSslError error(QSslError::NoError);

    foreach(QSslError error, errors)
        if(error.error() != QSslError::NoError)
            UpdateNode::Logging() << error.errorString();

    QList<QSslError> expectedSslErrors;
    expectedSslErrors.append(error);
    reply->ignoreSslErrors(expectedSslErrors);
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

