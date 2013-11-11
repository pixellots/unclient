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
