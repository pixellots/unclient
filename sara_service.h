#ifndef _SERVICE_H
#define _SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "downloader.h"

#define SARA_SERVICE_URL "http://10.20.2.199/sara/service"

namespace Sara
{
    class Service : public QObject
    {
        Q_OBJECT

        public:
            Service(QObject* parent);
            ~Service();

            bool checkForUpdates();
            int  returnCode();

        public slots:
            void requestReceived(QNetworkReply* reply);

        signals:
            void done();

        private:
            QNetworkAccessManager* m_pManager;
            Sara::Downloader* m_pDownloader;
    };
}

#endif // _SERVICE_H
