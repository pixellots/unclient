#ifndef _SERVICE_H
#define _SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "config.h"
#include "downloader.h"

#define UPDATENODE_SERVICE_URL            "https://updatenode.com/sara/service"

namespace UpdateNode
{
    class Service : public QObject
    {
        Q_OBJECT

        public:
            Service(QObject* parent);
            ~Service();

            enum Status { NOTHING = 0, UPDATE, MESSAGE, UPDATES, MESSAGES,
                          UPDATE_MESSAGE, UPDATE_MESSAGES, UPDATES_MESSAGE, UPDATES_MESSAGES };

            bool checkForUpdates();
            bool checkForUpdates(UpdateNode::Config* aConfig);

            int returnCodeManager();
            int returnCode(UpdateNode::Config* config = NULL);
            int returnCode(int aUpdateCount, int aMessageCode);

            QString notificationText(UpdateNode::Config* config = NULL);
            QString notificationTextManager();
        public slots:
            void requestReceived(QNetworkReply* reply);

        signals:
            void done();
            void doneManager();

        private:
            QNetworkAccessManager* m_pManager;
            UpdateNode::Downloader* m_pDownloader;
            QMap<QNetworkReply*, Config*> m_mapConfig;
    };
}

#endif // _SERVICE_H
