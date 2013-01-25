#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>
#include <QTextEdit>
#include "sara_service.h"

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
        void updateUpdateView();
        void updateMessageView();

    protected:
        void changeEvent(QEvent *e);

    public slots:
        void serviceDone();
        void refresh();
        void openLink(const QUrl& aUrl);
        void updateSelectedUpdate();
        void updateSelectedMessage();
        void startInstall();
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadDone(const Sara::Update& aUpdate);

        void processError();
        void processOutput();
        void updateExit(int aExitCode, QProcess::ExitStatus aExitStatus);

    private:
        Ui::DialogUpdate* m_pUI;
        Sara::Service* m_pService;
        Sara::Downloader* m_pDownloader;
        QProcess* m_pProcess;

        QList<Sara::Update> m_oReadyUpdates;
        Sara::Update m_oCurrentUpdate;

        QTextEdit m_oTextEdit;
};

#endif // DIALOG_H
