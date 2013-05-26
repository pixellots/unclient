#include "singleappdialog.h"
#include "ui_singleappdialog.h"
#include "config.h"
#include "settings.h"
#include "product.h"
#include "localfile.h"
#include "usernotofication.h"

#include <QtDebug>
#include <QMessageBox>

SingleAppDialog::SingleAppDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    m_pUi(new Ui::SingleAppDialog)
{
    m_pUi->setupUi(this);

    m_pUi->textProgress->setHidden(true);

    connect(m_pUi->chkDetails, SIGNAL(stateChanged(int)), SLOT(onDetailsCheck()));

    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processError()));
    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processOutput()));
    connect(&m_oCommander, SIGNAL(updateExit(int, QProcess::ExitStatus)), this, SLOT(updateExit(int, QProcess::ExitStatus)));

    m_pDownloader = new Sara::Downloader();

    connect(m_pDownloader, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64, qint64)));
    connect(m_pDownloader, SIGNAL(done(const Sara::Update&, QNetworkReply::NetworkError, const QString&)), SLOT(downloadDone(const Sara::Update&, QNetworkReply::NetworkError, const QString&)));

}

SingleAppDialog::~SingleAppDialog()
{
    if(m_pDownloader->isDownloading())
        m_pDownloader->cancel();

    delete m_pUi;
}

void SingleAppDialog::init(Sara::Service* aService)
{
    m_pService = aService;
    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
}

void SingleAppDialog::download()
{
    QList<Sara::Update> update_list = Sara::Config::Instance()->updates();
    for(int i = 0; i < update_list.size(); i++)
    {
        if(update_list.size() == 1)
            m_pUi->labelProgress->setText(tr("Downloading Update ..."));
        else
            m_pUi->labelProgress->setText(tr("Downloading Updates"));

        m_pDownloader->doDownload(update_list.at(i).getDownloadLink(), update_list.at(i));
    }
}

void SingleAppDialog::install()
{
    if(m_oReadyUpdates.size()==0)
    {
        /// TODO ... should not happen
        return;
    }

    m_oCurrentUpdate = m_oReadyUpdates.takeFirst();

    if(!m_oCommander.run(m_oCurrentUpdate))
    {
        /// TODO .... needs a check or somthing else
    }
}

void SingleAppDialog::serviceDone()
{
    Sara::Config* config = Sara::Config::Instance();
    setWindowTitle(config->product().getName() + tr(" - Update Client"));

    if(!config->product().getIconUrl().isEmpty())
    {
        if(!config->mainIcon().isEmpty())
            setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        else
            setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    }

    UserNotofication userNotify;

    userNotify.updateView();
    if(userNotify.exec() != QDialog::Accepted)
    {
        reject();
        return;
    }

    adjustSize();
    show();

    download();
}

void SingleAppDialog::onDetailsCheck()
{
    adjustSize();
}

void SingleAppDialog::processError()
{
    m_pUi->textProgress->setTextColor(Qt::red);
    m_pUi->textProgress->append(m_oCommander.readStdErr());
    m_pUi->textProgress->show();
}

void SingleAppDialog::processOutput()
{
    m_pUi->textProgress->setTextColor(Qt::black);
    m_pUi->textProgress->append(m_oCommander.readStdOut());
    m_pUi->textProgress->show();
}

void SingleAppDialog::updateExit(int aExitCode, QProcess::ExitStatus aExitStatus)
{
    Sara::Settings settings;

    if(aExitStatus == QProcess::NormalExit)
    {
        if(aExitCode == 0)
        {
            //m_pUI->labelProgress->setText(tr("Update '%1' installed successfully").arg(m_oCurrentUpdate.getTitle()));
            qDebug() << m_oCurrentUpdate.getTitle() << " updated successfully!";

            if(m_oCurrentUpdate.getTypeEnum() == Sara::Update::CLIENT_SETS_VERSION)
                settings.setNewVersion(Sara::Config::Instance()->product(), m_oCurrentUpdate.getTargetVersion());

            //install();
        }
        else
        {
            //m_pUI->pshUpdate->show();
            //m_pUI->pshCheck->show();
            //m_pUI->labelProgress->setText(tr("Update '%1' failed with error %2").arg(m_oCurrentUpdate.getTitle()).arg(aExitCode));

            qDebug() << m_oCurrentUpdate.getTitle() << " updated failed!";

        }
    }
    else
    {
        qDebug() << m_oCurrentUpdate.getTitle() << " carshed!";
    }

    settings.setUpdate(m_oCurrentUpdate, Sara::LocalFile::getDownloadLocation(m_oCurrentUpdate.getDownloadLink()), aExitCode);
}

void SingleAppDialog::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_pUi->progressBar->maximum() <= bytesTotal)
    {
        m_pUi->progressBar->setRange(0, bytesTotal);
        m_pUi->progressBar->setValue(bytesReceived);
    }
}

void SingleAppDialog::downloadDone(const Sara::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString)
{
    m_oReadyUpdates.append(aUpdate);

    if(aError != QNetworkReply::NoError)
    {
        qDebug() << "Download failed: " << aErrorString;
        return;
    }

    if(!m_pDownloader->isDownloading())
    {

        install();
    }
}
