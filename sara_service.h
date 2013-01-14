#ifndef _SERVICE_H
#define _SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#define SARA_SERVICE_URL "http://192.168.1.102/sara/index.php/service"

namespace Sara
{
    class Service : public QObject
    {
        Q_OBJECT

        public:
            Service(QObject* parent);
            ~Service();

            bool checkForUpdates();

        public slots:
            void requestReceived(QNetworkReply* reply);

        signals:
            void done();

        private:
            QNetworkAccessManager* m_pManager;
    };
}

#endif // _SERVICE_H
