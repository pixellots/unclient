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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>
#include <QTextEdit>
#include <QTreeWidgetItem>
#include "updatenode_service.h"
#include "commander.h"

namespace Ui
{
    class DialogUpdate;
}

class MultiAppDialog : public QDialog
{
    Q_OBJECT
    public:
        MultiAppDialog(QWidget *parent = 0);
        ~MultiAppDialog();

        void init(UpdateNode::Service* aService);
        void initView();

    private:
        void install();
        void updateView(UpdateNode::Config* aConfig = NULL);
        void updateCounter();

    protected:
        void changeEvent(QEvent *e);

    public slots:
        void serviceDone();
        void serviceDoneManager();
        void refresh();
        void cancelProgress();
        void contextMenu(const QPoint& pos);
        void openLink(const QUrl& aUrl);
        void updateSelectedUpdate();
        void startInstall();
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadDone(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString);
        void checkSelection();
        void onClose();

        void processError();
        void processOutput();
        void updateExit(int aExitCode, QProcess::ExitStatus aExitStatus);

    private:
        Ui::DialogUpdate* m_pUI;
        UpdateNode::Service* m_pService;
        UpdateNode::Downloader* m_pDownloader;

        QList<UpdateNode::Update> m_oReadyUpdates;
        UpdateNode::Update m_oCurrentUpdate;
        UpdateNode::Commander m_oCommander;
        QTreeWidgetItem* m_pCurrentItem;
        QTreeWidgetItem* m_pIgnoredItem;

        QTextEdit m_oTextEdit;
        QString m_strErrorString;
        int m_iError;

        int m_iNewUpdates;
        bool m_bIsInstalling;
};

#endif // DIALOG_H
