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

namespace Sara
{

    class Downloader: public QObject
     {
         Q_OBJECT

         public:
             Downloader();
             Downloader(const QString& aTarget);

        public:
             void doDownload(const QUrl& url, const Sara::Update& aUpdate);
             bool saveToDisk(const QString &filename, QIODevice *data);
             void setTarget(const QString& aTarget);
             QString getTarget() const;

             bool isDownloading();

         public slots:
             void downloadFinished(QNetworkReply *reply);

        signals:
             void done(const Sara::Update& aUpdate);
             void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

        private:
             QNetworkAccessManager m_oManager;
             QMap<QNetworkReply*, Sara::Update> m_oCurrentDownloads;

             QString m_strTarget;
     };


}
#endif // DOWNLOADER_H
