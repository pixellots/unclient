#include <QDir>
#include "logging.h"
#include "downloader.h"
#include "localfile.h"
#include "settings.h"
#include "status.h"

using namespace UpdateNode;

Downloader::Downloader()
{
    connect(&m_oManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
}

void Downloader::doDownload(const QUrl& url, const UpdateNode::Update& aUpdate)
{
    UpdateNode::Settings settings;
    QString cachedFile = settings.getCachedFile(aUpdate.getCode());
    if(!cachedFile.isEmpty() && QFile::exists(cachedFile))
    {
        emit done(aUpdate, QNetworkReply::NoError, QString());
        return;
    }

    QNetworkRequest request(url);

    QNetworkReply *reply = m_oManager.get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SIGNAL(downloadProgress(qint64,qint64)));

    m_oCurrentDownloads[reply] = aUpdate;
}

void Downloader::cancel()
{
    QMapIterator<QNetworkReply*, UpdateNode::Update> i(m_oCurrentDownloads);
    while (i.hasNext())
    {
         i.next();
         i.key()->abort();
    }
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data, const QString& aCode)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        fprintf(stderr, "Could not open %s for writing: %s\n",
             qPrintable(filename),
             qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    UpdateNode::Settings settings;
    settings.setCachedFile(aCode, filename);

    return true;
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    QNetworkReply::NetworkError error;
    QString errorString;

    error = reply->error();
    errorString = reply->errorString();
    UpdateNode::Update update = m_oCurrentDownloads.value(reply);

    QUrl url = reply->url();
    if (reply->error() != QNetworkReply::NoError)
        UpdateNode::Logging() << "Download of " << url.toEncoded().constData() << " failed: " << reply->errorString();
    else
    {
        QString filename;
        filename = UpdateNode::LocalFile::getDownloadLocation(url.toString());

        if (saveToDisk(filename, reply, update.getCode()))
            UpdateNode::Logging() << "Download of " << url.toEncoded().constData() << " succeeded (saved to " << filename << ")";
    }

    m_oCurrentDownloads.remove(reply);
    reply->deleteLater();

    emit done(update, error, errorString);
}

bool Downloader::isDownloading()
{
    return m_oCurrentDownloads.size() > 0;
}
