#ifndef _SERVICE_H
#define _SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "config.h"
#include "downloader.h"

#define SARA_SERVICE_URL            "http://localhost/sara/service"

namespace Sara
{
    class Service : public QObject
    {
        Q_OBJECT

        public:
            Service(QObject* parent);
            ~Service();

            bool checkForUpdates();
            bool checkForUpdates(Sara::Config* aConfig);
            int  returnCode();

        public slots:
            void requestReceived(QNetworkReply* reply);

        signals:
            void done();
            void doneManager();

        private:
            QNetworkAccessManager* m_pManager;
            Sara::Downloader* m_pDownloader;
            QMap<QNetworkReply*, Config*> m_mapConfig;
    };
}

#endif // _SERVICE_H
