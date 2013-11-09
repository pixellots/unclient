#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QMap>

#include <stdio.h>

#include "update.h"

namespace UpdateNode
{

    class Downloader: public QObject
     {
         Q_OBJECT

         public:
             Downloader();

        public:
             QNetworkReply* doDownload(const QUrl& url, const UpdateNode::Update& aUpdate);
             bool saveToDisk(const QString &filename, QIODevice *data, const QString& aCode);
             bool isDownloading();

             void cancel();

         public slots:
             void downloadFinished(QNetworkReply *reply);

        signals:
             void done(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString);
             void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

        private:
             QNetworkAccessManager m_oManager;
             QMap<QNetworkReply*, UpdateNode::Update> m_oCurrentDownloads;
     };


}
#endif // DOWNLOADER_H
