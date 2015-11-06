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

#include <QDir>
#include <QDebug>
#if QT_VERSION >= 0x040700
#include <QNetworkConfigurationManager>
#endif
#include "logging.h"
#include "downloader.h"
#include "localfile.h"
#include "settings.h"
#include "status.h"
#include "security.h"

using namespace UpdateNode;

/*!
\class UpdateNode::Downloader
\brief Main class for downloading files from the internet
\n\n
This class is responsible for downloading all data needed for your update: Update file, product images, etc.
*/

/*!
Constructs an empty Downloader object.
*/
Downloader::Downloader()
{
#if QT_VERSION >= 0x040700
    QNetworkConfigurationManager manager;
    m_oManager.setConfiguration(manager.defaultConfiguration());
#endif
}

/*!
Starts a download for a specified url with an reference to a file name specified by aFileName
\n
\note aFileName is only used as an reference for the done(QByteArray array, const QString& fileName) signal
\sa Downloader::downloadFileFinished
*/
void Downloader::doDownload(const QUrl& url, const QString& aFileName)
{
    QNetworkRequest request(url);

    connect(&m_oManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFileFinished(QNetworkReply*)));
    connect(&m_oManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(onSslError(QNetworkReply*,QList<QSslError>)));
#if QT_VERSION >= 0x040700
    connect(&m_oManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)), SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
#endif
    QNetworkReply *reply = m_oManager.get(request);

    m_oCurrentFileDownloads[reply] = aFileName;
}

/*!
Starts a download for a specified url with an reference to Update object specified by aUpdate
\n
\note aUpdate is only used as an reference for the
\note done(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString) signal
\sa Downloader::downloadFinished
*/
QNetworkReply* Downloader::doDownload(const QUrl& url, const UpdateNode::Update& aUpdate)
{
    QUrl finalUrl = url;
    UpdateNode::Settings settings;
    QString cachedFile = settings.getCachedFile(aUpdate.getCode());

    if(!cachedFile.isEmpty() && QFileInfo(cachedFile).exists())
    {
        if(!aUpdate.getChecksum().isEmpty())
            if(!UpdateNode::Security::validateChecksum(cachedFile, aUpdate.getChecksum(), aUpdate.getChecksumAlg()))
                QFile::remove(cachedFile);

        if(QFile::exists(cachedFile))
        {
            emit done(aUpdate, QNetworkReply::NoError, QString());
            return NULL;
        }
    }

    finalUrl.setUserName(QUrl::fromEncoded(finalUrl.userName().toLatin1()).toString());
    finalUrl.setPassword(QUrl::fromEncoded(finalUrl.password().toLatin1()).toString());

    connect(&m_oManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    connect(&m_oManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(onSslError(QNetworkReply*,QList<QSslError>)));
#if QT_VERSION >= 0x040700
    connect(&m_oManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)), SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
#endif
    QNetworkRequest request(finalUrl);

    QNetworkReply *reply = m_oManager.get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SIGNAL(downloadProgress(qint64,qint64)));

    m_oCurrentDownloads[reply] = aUpdate;

    return reply;
}

/*!
Aborts all current network operations
*/
void Downloader::cancel()
{
    QMapIterator<QNetworkReply*, UpdateNode::Update> i(m_oCurrentDownloads);
    while (i.hasNext())
    {
         i.next();
         i.key()->abort();
    }
}

/*!
Saves all data specified by \adata into the given \afilename and stores the cache referenced by \aaCode
\n
returns true on success, otherwise false
*/
bool Downloader::saveToDisk(const QString &filename, QIODevice *data, const QString& aCode)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        UpdateNode::Logging() << "Could not open " << filename <<  " for writing: " << file.errorString();
        return false;
    }

    QByteArray buffer;
    int chunksize = 1024 * 1024;
    while (!(buffer = data->read(chunksize)).isEmpty())
    {
        if(file.write(buffer) == -1)
        {
            UpdateNode::Logging() << "File operation failed for " << filename <<  ": " << file.errorString();
            return false;
        }
        file.flush();
    #ifdef Q_OS_LINUX
        fsync(file.handle());
    #endif

    }
    file.close();

    UpdateNode::Settings settings;
    settings.setCachedFile(aCode, filename);

    return true;
}

/*!
Slot for doDownload on a file, emits done(QByteArray array, const QString& fileName)
*/
void Downloader::downloadFileFinished(QNetworkReply *reply)
{
    if(reply->canReadLine())
    {
        if(reply->error() == QNetworkReply::NoError)
            emit done(reply->readAll(), m_oCurrentFileDownloads.take(reply));
        else
            UpdateNode::Logging() << m_oCurrentFileDownloads.take(reply) << " failed: " << reply->errorString();

        reply->deleteLater();
    }

}

/*!
Slot for doDownload on a Update object, emits
\n
done(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString)
\note This method stored the downloaded file using UpdateNode::LocalFile::getDownloadLocation function
*/
void Downloader::downloadFinished(QNetworkReply *reply)
{
    QNetworkReply::NetworkError error;
    QString errorString;

    error = reply->error();
    errorString = reply->errorString();

    if(!m_oCurrentDownloads.contains(reply))
        return;

    UpdateNode::Update update = m_oCurrentDownloads.value(reply);

    QUrl url = reply->url();
    if (reply->error() != QNetworkReply::NoError)
        UpdateNode::Logging() << "Download of " << url.toEncoded().constData() << " failed: " << reply->errorString();
    else
    {
        reply->waitForReadyRead(-1);

        QString filename;
        filename = UpdateNode::LocalFile::getDownloadLocation(url.toString());

        saveToDisk(filename, reply, update.getCode());

        if(!update.getChecksum().isEmpty())
        {
            if(!UpdateNode::Security::validateChecksum(filename, update.getChecksum(), update.getChecksumAlg()))
            {
                error = QNetworkReply::UnknownContentError;
                errorString = tr("Invalid checksum. Try again later, or check your internet connection.");
                QFile::remove(filename);
            }
        }
    }

    m_oCurrentDownloads.remove(reply);

    if(!isDownloading())
        emit done(update, error, errorString);

    reply->deleteLater();
}

/*!
Checking if there are currently any downloads in progess, or not
*/
bool Downloader::isDownloading()
{
    return m_oCurrentDownloads.size() > 0;
}

/*!
Slot called on SSL errors
*/
void Downloader::onSslError(QNetworkReply *reply, const QList<QSslError>& errors)
{
    QSslError error(QSslError::NoError);

    foreach(QSslError error, errors)
        if(error.error() != QSslError::NoError)
            UpdateNode::Logging() << error.errorString();

    QList<QSslError> expectedSslErrors;
    expectedSslErrors.append(error);
    reply->ignoreSslErrors(expectedSslErrors);
}

#if QT_VERSION >= 0x040700
/*!
Slot called network state changes
*/
void Downloader::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility status)
{
    if(status == QNetworkAccessManager::NotAccessible)
    {
        UpdateNode::Logging() << "Connection lost. Download aborted.";
        cancel();
        qApp->exit(UPDATENODE_PROCERROR_CONNECTION_ERROR);
    }
}
#endif

