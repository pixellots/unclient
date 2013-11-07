#ifndef SINGLEAPPDIALOG_H
#define SINGLEAPPDIALOG_H

#include <QDialog>
#include <QList>
#include "updatenode_service.h"
#include "commander.h"

namespace Ui
{
    class SingleAppDialog;
}

class SingleAppDialog : public QDialog
{
    Q_OBJECT  
    public:
        explicit SingleAppDialog(QWidget *parent = 0);
        ~SingleAppDialog();

        void init(UpdateNode::Service* aService, bool aDownloadOnly, bool aExecuteOnly);
        void download();
        void install();

        static bool toAssending(const UpdateNode::Update& a, const UpdateNode::Update& b);

    public slots:
        void serviceDone();
        void onDetailsCheck();
        void onCancel();

        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadDone(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString);

        void processError();
        void processOutput();
        void updateExit(int aExitCode, QProcess::ExitStatus aExitStatus);

    private:
        Ui::SingleAppDialog* m_pUi;
        UpdateNode::Service* m_pService;
        UpdateNode::Downloader* m_pDownloader;

        QList<UpdateNode::Update> m_oReadyUpdates;
        UpdateNode::Update m_oCurrentUpdate;
        UpdateNode::Commander m_oCommander;
        bool m_bDownloadOnly;
        bool m_bExecuteOnly;
        int  m_iErrorCode;
};

#endif // SINGLEAPPDIALOG_H
