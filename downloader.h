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

#include <stdio.h>

namespace Sara
{

    class Downloader: public QObject
     {
         Q_OBJECT

         public:
             Downloader();
             Downloader(const QString& aTarget);

        public:
             void doDownload(const QUrl &url);
             bool saveToDisk(const QString &filename, QIODevice *data);

         public slots:
             void execute();
             void downloadFinished(QNetworkReply *reply);

        signals:
             void done();

        private:
             QNetworkAccessManager manager;
             QList<QNetworkReply *> currentDownloads;

             QString m_strTarget;
     };


}
#endif // DOWNLOADER_H
