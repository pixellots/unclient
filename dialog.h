#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>
#include <QTextEdit>
#include <QTreeWidgetItem>
#include "sara_service.h"
#include "commander.h"

namespace Ui
{
    class DialogUpdate;
}

class Dialog : public QDialog
{
    Q_OBJECT
    public:
        Dialog(QWidget *parent = 0);
        ~Dialog();

        void init(Sara::Service* aService);

    private:
        void install();
        void updateUpdateView(Sara::Config* aConfig = NULL);
        void updateMessageView(Sara::Config* aConfig = NULL);
        void updateTabCounter(bool aChangeTab = true);
        void resetMessageItem(QTreeWidgetItem* aItem);

    protected:
        void changeEvent(QEvent *e);

    public slots:
        void serviceDone();
        void serviceDoneManager();
        void refresh();
        void cancelProgress();
        void openLink(const QUrl& aUrl);
        void updateSelectedUpdate();
        void updateSelectedMessage();
        void startInstall();
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadDone(const Sara::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString);
        void messageLoaded(bool aSuccess);
        void tabSelected(int aIndex);
        void checkSelection();

        void processError();
        void processOutput();
        void updateExit(int aExitCode, QProcess::ExitStatus aExitStatus);

    private:
        Ui::DialogUpdate* m_pUI;
        Sara::Service* m_pService;
        Sara::Downloader* m_pDownloader;

        QList<Sara::Update> m_oReadyUpdates;
        Sara::Update m_oCurrentUpdate;
        Sara::Commander m_oCommander;

        QTextEdit m_oTextEdit;

        int m_iNewUpdates;
        int m_iNewMessages;
};

#endif // DIALOG_H
