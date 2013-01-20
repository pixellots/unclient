#include <QDir>
#include "downloader.h"

using namespace Sara;

Downloader::Downloader()
{
    connect(&m_oManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
}

Downloader::Downloader(const QString& aTarget)
{
    m_strTarget = aTarget;

    connect(&m_oManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
}

void Downloader::setTarget(const QString& aTarget)
{
    m_strTarget = aTarget;
}

QString Downloader::getTarget() const
{
    return m_strTarget;
}

void Downloader::doDownload(const QUrl& url, const Sara::Update& aUpdate)
{
    QNetworkRequest request(url);

    QNetworkReply *reply = m_oManager.get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SIGNAL(downloadProgress(qint64,qint64)));

    m_oCurrentDownloads[reply] = aUpdate;
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data)
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

    return true;
}

void Downloader::downloadFinished(QNetworkReply *reply)
{

    QUrl url = reply->url();
    if (reply->error())
    {
        fprintf(stderr, "Download of %s failed: %s\n",
             url.toEncoded().constData(),
             qPrintable(reply->errorString()));
    }
    else
    {
        QString filename;
        if(!getTarget().isEmpty())
            filename = m_strTarget;
        {
            if(!QDir(QDir::tempPath() + QDir::separator() + "Sara").exists())
                QDir(QDir::tempPath()).mkdir("Sara");
            filename = QDir::tempPath() + QDir::separator() + "Sara" + QDir::separator() + QFileInfo(url.toString()).fileName();
        }

        if (saveToDisk(filename, reply))
            printf("Download of %s succeeded (saved to %s)\n",
                url.toEncoded().constData(), qPrintable(filename));
    }

    Sara::Update update = m_oCurrentDownloads.value(reply);
    m_oCurrentDownloads.remove(reply);
    reply->deleteLater();

    emit done(update);
}

bool Downloader::isDownloading()
{
    return m_oCurrentDownloads.size() > 0;
}
