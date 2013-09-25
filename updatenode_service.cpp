#include <stdlib.h>
#include <QtDebug>
#include <QUrl>
#include <QNetworkReply>
#include <QLocale>

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

    if(!globalConfig->getLanguage().isEmpty())
        url.addQueryItem("lang", globalConfig->getLanguage());
    else
        url.addQueryItem("lang", QLocale::system().name());

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
    if(UpdateNode::Config::Instance()->updates().size() == 0 && UpdateNode::Config::Instance()->messages().size() == 0)
        return 0; // nothing
    else if(UpdateNode::Config::Instance()->updates().size() == 1 && UpdateNode::Config::Instance()->messages().size() == 0)
        return 1; // one update
    else if(UpdateNode::Config::Instance()->updates().size() == 0 && UpdateNode::Config::Instance()->messages().size() == 1)
        return 2; // one message
    else if(UpdateNode::Config::Instance()->updates().size() > 1 && UpdateNode::Config::Instance()->messages().size() == 0)
        return 3; // multiple updates
    else if(UpdateNode::Config::Instance()->updates().size() == 0 && UpdateNode::Config::Instance()->messages().size() > 1)
        return 4; // multiple messages
    else if(UpdateNode::Config::Instance()->updates().size() == 1 && UpdateNode::Config::Instance()->messages().size() == 1)
        return 5; // one update & one message
    else if(UpdateNode::Config::Instance()->updates().size() == 1 && UpdateNode::Config::Instance()->messages().size() > 1)
        return 6; // one update & multiple messages
    else if(UpdateNode::Config::Instance()->updates().size() > 1 && UpdateNode::Config::Instance()->messages().size() == 1)
        return 7; // multiple updates & one message
    else
        return 8; // multiple updates & messages
}
