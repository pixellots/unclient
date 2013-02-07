#include <stdlib.h>
#include <QtDebug>
#include <QUrl>
#include <QNetworkReply>

#include "sara_service.h"
#include "config.h"
#include "settings.h"
#include "version.h"
#include "xmlparser.h"
#include "product.h"
#include "osdetection.h"

using namespace Sara;

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
    Sara::Config* config = Sara::Config::Instance();
    Sara::Settings settings;

    if(!m_pManager)
    {
        m_pManager = new QNetworkAccessManager(this);

        connect(m_pManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(requestReceived(QNetworkReply*)));
    }

    QNetworkRequest request;
    QUrl url(SARA_SERVICE_URL);

    url.addQueryItem("key", config->getKey());
    url.addQueryItem("id", settings.uuid());
    url.addQueryItem("os", Sara::OSDetection::getOS());

    if(settings.getVersionCode().isEmpty())
    {
        url.addQueryItem("productCode", settings.getProductCode());
        url.addQueryItem("productVersion", settings.getProductVersion());
    }
    else
        url.addQueryItem("versionCode", settings.getVersionCode());

    qDebug() << "REQUEST: " << url.toString();
    request.setUrl(url);
    request.setRawHeader("User-Agent", QString("SaraClient %1 (%2)").arg(SARA_CLIENT_VERSION).arg(config->getOS()).toAscii());

    m_pManager->get(request);

    return true;
}

void Service::requestReceived(QNetworkReply* reply)
{
    reply->deleteLater();

    if(reply->error() == QNetworkReply::NoError)
    {
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            QString replyText = reply->readAll();

            Sara::XmlParser* parser = new Sara::XmlParser(this);
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

    if(!Sara::Config::Instance()->product().getIconUrl().isEmpty())
    {
        if(!m_pDownloader)
            m_pDownloader = new Sara::Downloader(Sara::Config::Instance()->product().getLocalIcon());

        connect(m_pDownloader, SIGNAL(done(const Sara::Update&, QNetworkReply::NetworkError, const QString&)), this, SIGNAL(done()));

        m_pDownloader->doDownload(Sara::Config::Instance()->product().getIconUrl(), Sara::Update());
    }
    else
        emit done();
}
