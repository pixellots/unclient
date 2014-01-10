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

#include <QDir>
#include <QDebug>
#include "logging.h"
#include "downloader.h"
#include "localfile.h"
#include "settings.h"
#include "status.h"

using namespace UpdateNode;

/*!
\class UpdateNode::Downloader
\brief Downloader class for downloading any content from the web
*/

/*!
Constructs a Downloader object
*/
Downloader::Downloader()
{
}

/*!
Downloads a file from a given URL and stores the file name for future use
*/
void Downloader::doDownload(const QUrl& url, const QString& aFileName)
{
    QNetworkRequest request(url);

    connect(&m_oManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFileFinished(QNetworkReply*)));

    QNetworkReply *reply = m_oManager.get(request);

    m_oCurrentFileDownloads[reply] = aFileName;
}

/*!
Downloads a file from a given URL based on an Update object for future use\n
This function returns the QNetworkReply pointer to the request
*/
QNetworkReply* Downloader::doDownload(const QUrl& url, const UpdateNode::Update& aUpdate)
{
    UpdateNode::Settings settings;
    QString cachedFile = settings.getCachedFile(aUpdate.getCode());
    if(!cachedFile.isEmpty() && QFile::exists(cachedFile))
    {
        emit done(aUpdate, QNetworkReply::NoError, QString());
        return NULL;
    }

    connect(&m_oManager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));

    QNetworkRequest request(url);

    QNetworkReply *reply = m_oManager.get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SIGNAL(downloadProgress(qint64,qint64)));

    m_oCurrentDownloads[reply] = aUpdate;

    return reply;
}

/*!
Cancel all current downloads
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
Saves data to a file specified with filename and stores the filename and aCode in the registry
*/
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

/*!
This slot is called when the doDownlaod method for single files is finished
*/
void Downloader::downloadFileFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
        emit done(reply->readAll(), m_oCurrentFileDownloads.take(reply));

    reply->deleteLater();
}

/*!
This slot is called when the doDownlaod method for an update file is finished
*/
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

/*!
Checkes whether there is still a downlaod in progress.\n
returns true if no futher download is in progress, otherwise false
*/
bool Downloader::isDownloading()
{
    return m_oCurrentDownloads.size() > 0;
}
