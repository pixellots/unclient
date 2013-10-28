#include <stdlib.h>
#include <QtDebug>
#include <QUrl>
#include <QNetworkReply>

#include "updatenode_service.h"
#include "config.h"
#include "settings.h"
#include "version.h"
#include "xmlparser.h"
#include "product.h"
#include "osdetection.h"

using namespace UpdateNode;

Service::Service(QObject* parent)
    : QObject(parent)
{
    m_pManager = NULL;
    m_pDownloader = NULL;
}

Service::~Service()
{
    m_pManager->deleteLater();
    m_pDownloader->deleteLater();
}

bool Service::checkForUpdates()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

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

bool Service::checkForUpdates(UpdateNode::Config* aConfig)
{
    UpdateNode::Config* globalConfig = UpdateNode::Config::Instance();
    UpdateNode::Settings settings;

    QNetworkRequest request;
    QUrl url(UPDATENODE_SERVICE_URL);

    if(!globalConfig->getHost().isEmpty())
        url.setHost(globalConfig->getHost());

    url.addQueryItem("key", globalConfig->getKey());

    if(!globalConfig->getTestKey().isEmpty())
        url.addQueryItem("test", globalConfig->getTestKey());

    url.addQueryItem("id", settings.uuid());
    url.addQueryItem("os", UpdateNode::OSDetection::getOS());
    url.addQueryItem("arch", UpdateNode::OSDetection::getArch());
    url.addQueryItem("lang", globalConfig->getLanguage());

    if(settings.getVersionCode().isEmpty())
    {
        url.addQueryItem("productCode", settings.getProductCode(aConfig));
        url.addQueryItem("productVersion", settings.getProductVersion(aConfig));
    }
    else
        url.addQueryItem("versionCode", settings.getVersionCode());

    qDebug() << "REQUEST: " << url.toString();
    request.setUrl(url);
    request.setRawHeader("charset", "utf-8" );
    request.setRawHeader("User-Agent", QString("UpdateNodeClient %1 (%2)").arg(UPDATENODE_CLIENT_VERSION).arg(globalConfig->getOS()).toAscii());

    QNetworkReply* reply = m_pManager->get(request);

    m_mapConfig[reply] = aConfig;

    return true;
}

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
            qDebug() << "RESULT: " << parser->getStatusString() << "(" << parser->getStatus() << ")";
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
        qDebug() << "ERROR: " << reply->errorString();
    }

    if(!config->product().getIconUrl().isEmpty())
    {
        if(!m_pDownloader)
            m_pDownloader = new UpdateNode::Downloader();

        m_pDownloader->setTarget(config->product().getLocalIcon());

        if(UpdateNode::Config::Instance()->isSingleMode())
            connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), this, SIGNAL(done()));
        else
            connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), this, SIGNAL(doneManager()));

        m_pDownloader->doDownload(config->product().getIconUrl(), UpdateNode::Update());

    }
    else
    {
        if(UpdateNode::Config::Instance()->isSingleMode())
            emit done();
        else
            emit doneManager();
    }
}

int Service::returnCode()
{
    Service::Status status;
    if(UpdateNode::Config::Instance()->updates().size() == 0 && UpdateNode::Config::Instance()->messages().size() == 0)
        status = Service::NOTHING; // nothing
    else if(UpdateNode::Config::Instance()->updates().size() == 1 && UpdateNode::Config::Instance()->messages().size() == 0)
        status = Service::UPDATE; // one update
    else if(UpdateNode::Config::Instance()->updates().size() == 0 && UpdateNode::Config::Instance()->messages().size() == 1)
        status = Service::MESSAGE; // one message
    else if(UpdateNode::Config::Instance()->updates().size() > 1 && UpdateNode::Config::Instance()->messages().size() == 0)
        status = Service::UPDATES; // multiple updates
    else if(UpdateNode::Config::Instance()->updates().size() == 0 && UpdateNode::Config::Instance()->messages().size() > 1)
        status = Service::MESSAGES; // multiple messages
    else if(UpdateNode::Config::Instance()->updates().size() == 1 && UpdateNode::Config::Instance()->messages().size() == 1)
        status = Service::UPDATE_MESSAGE; // one update & one message
    else if(UpdateNode::Config::Instance()->updates().size() == 1 && UpdateNode::Config::Instance()->messages().size() > 1)
        status = Service::UPDATE_MESSAGES; // one update & multiple messages
    else if(UpdateNode::Config::Instance()->updates().size() > 1 && UpdateNode::Config::Instance()->messages().size() == 1)
        status = Service::UPDATES_MESSAGE; // multiple updates & one message
    else
        status = Service::UPDATES_MESSAGES; // multiple updates & messages

    if(UpdateNode::Config::Instance()->isSystemTray())
    {
        switch(status)
        {
            case Service::MESSAGE:
            case Service::MESSAGES:
                return Service::NOTHING;
            case Service::UPDATE_MESSAGE:
            case Service::UPDATE_MESSAGES:
                return Service::UPDATE;
            case Service::UPDATES_MESSAGE:
            case Service::UPDATES_MESSAGES:
                return Service::UPDATES;
            default:
                return status;
       }
    }

    if(UpdateNode::Config::Instance()->isSingleMode())
    {
        switch(status)
        {
            case Service::UPDATES:
            case Service::UPDATES_MESSAGE:
            case Service::UPDATES_MESSAGES:
                return Service::UPDATE;
            default:
                return status;
       }
    }
    return status;
}

QString Service::notificationText()
{
    QString text;
    switch(returnCode())
    {
        case Service::NOTHING:
            text = QObject::tr("There are no new updates & messages available");
            break;
        case Service::UPDATE:
            text = QObject::tr("There is a new update available");
            break;
        case Service::MESSAGE:
            text = QObject::tr("There is a new message available");
            break;
        case Service::UPDATES:
            text = QObject::tr("There are multiple updates available");
            break;
        case Service::MESSAGES:
            text = QObject::tr("There are multiple messages available");
            break;
        case Service::UPDATE_MESSAGE:
            text = QObject::tr("There is an update and one message available");
            break;
        case Service::UPDATE_MESSAGES:
            text = QObject::tr("There is an update and multiple messages available");
            break;
        case Service::UPDATES_MESSAGE:
            text = QObject::tr("There are multiple updates and one message available");
            break;
        case Service::UPDATES_MESSAGES:
            text = QObject::tr("There are multiple updates and messages available");
            break;
        default:
            text = QObject::tr("Undefined state");
            break;
    }
    return text;
}
