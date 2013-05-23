#ifndef SINGLEAPPDIALOG_H
#define SINGLEAPPDIALOG_H

#include <QDialog>
#include <QList>
#include "sara_service.h"
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

        void init(Sara::Service* aService);

    public slots:
        void serviceDone();
        void onDetailsCheck();

    private:
        Ui::SingleAppDialog* m_pUi;
        Sara::Service* m_pService;
        Sara::Downloader* m_pDownloader;

        QList<Sara::Update> m_oReadyUpdates;
        Sara::Update m_oCurrentUpdate;
        Sara::Commander m_oCommander;
};

#endif // SINGLEAPPDIALOG_H
