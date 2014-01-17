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

#ifndef _SERVICE_H
#define _SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "config.h"
#include "downloader.h"
#include "status.h"

#define UPDATENODE_SERVICE_URL            "https://updatenode.com/sara/service"

namespace UpdateNode
{
    class Service : public QObject
    {
        Q_OBJECT

        public:
            Service(QObject* parent = 0);
            ~Service();

            enum Status { NOTHING = 0, UPDATE, MESSAGE, UPDATE_MESSAGE };

            bool checkForUpdates();
            bool checkForUpdates(UpdateNode::Config* aConfig);

            int status();
            QString statusText() const;

            int returnCodeManager();
            int returnCode(UpdateNode::Config* config = NULL);
            int returnCode(int aUpdateCount, int aMessageCount);

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
            QList<UpdateNode::Config*> m_listConfigs;

            int m_iStatus;
            QString m_strStatus;
    };
}

#endif // _SERVICE_H
