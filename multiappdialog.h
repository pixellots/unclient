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

    private:
        void install();
        void updateUpdateView(UpdateNode::Config* aConfig = NULL);
        void updateCounter();

    protected:
        void changeEvent(QEvent *e);

    public slots:
        void serviceDone();
        void serviceDoneManager();
        void refresh();
        void cancelProgress();
        void openLink(const QUrl& aUrl);
        void updateSelectedUpdate();
        void startInstall();
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadDone(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString);
        void checkSelection();

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
        QTreeWidgetItem* m_currentItem;

        QTextEdit m_oTextEdit;

        int m_iNewUpdates;
        bool m_bIsInstalling;
};

#endif // DIALOG_H
