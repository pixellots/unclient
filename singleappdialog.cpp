#include "singleappdialog.h"
#include "ui_singleappdialog.h"
#include "config.h"
#include "settings.h"
#include "product.h"
#include "localfile.h"
#include "usernotofication.h"
#include "status.h"

#include <QtDebug>
#include <QMessageBox>

SingleAppDialog::SingleAppDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    m_pUi(new Ui::SingleAppDialog),
    m_bDownloadOnly(false), m_bExecuteOnly(false)
{
    m_pUi->setupUi(this);

    m_pUi->textProgress->setHidden(true);

    connect(m_pUi->chkDetails, SIGNAL(stateChanged(int)), SLOT(onDetailsCheck()));

    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processError()));
    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processOutput()));
    connect(&m_oCommander, SIGNAL(updateExit(int, QProcess::ExitStatus)), this, SLOT(updateExit(int, QProcess::ExitStatus)));

    m_pDownloader = new UpdateNode::Downloader();

    connect(m_pDownloader, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64, qint64)));
    connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), SLOT(downloadDone(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)));

    m_pUi->chkDetails->hide();
}

SingleAppDialog::~SingleAppDialog()
{
    if(m_pDownloader->isDownloading())
        m_pDownloader->cancel();

    delete m_pUi;
}

void SingleAppDialog::init(UpdateNode::Service* aService, bool aDownloadOnly, bool aExecuteOnly)
{
    m_bDownloadOnly = aDownloadOnly;
    m_bExecuteOnly = aExecuteOnly;
    m_pService = aService;
    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
}

void SingleAppDialog::download()
{
    QList<UpdateNode::Update> update_list = UpdateNode::Config::Instance()->updates();
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
    if(m_oReadyUpdates.isEmpty() || m_bDownloadOnly)
    {
        accept();
        return;
    }

    m_oCurrentUpdate = m_oReadyUpdates.takeFirst();

    if(m_oCurrentUpdate.getTypeEnum() == UpdateNode::Update::INSTALLER_SETS_VERSION)
        hide();

    m_pUi->labelProgress->setText(tr("Installing Update ..."));

    if(!m_oCommander.run(m_oCurrentUpdate))
    {
        qApp->exit(UPDATENODE_PROCERROR_COMMAND_LAUNCH_FAILED);
        return;
    }

    if(m_oCurrentUpdate.getTypeEnum() == UpdateNode::Update::INSTALLER_SETS_VERSION)
        accept();
}

void SingleAppDialog::serviceDone()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();
    setWindowTitle(config->product().getName() + tr(" - Update Client"));

    if(!config->product().getIconUrl().isEmpty())
    {
        if(!config->mainIcon().isEmpty())
            setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        else
            setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    }

    if(config->updates().size()==0)
    {
        qApp->exit(UPDATENODE_PROCERROR_NO_UPDATES);
        return;
    }

    if(!m_bExecuteOnly)
    {
        UserNotofication userNotify;
        userNotify.updateView();
        if(userNotify.exec() != QDialog::Accepted)
        {
            qApp->exit(UPDATENODE_PROCERROR_CANCELED);
            return;
        }

        adjustSize();
        show();

        download();
    }
    else
    {
        QList<UpdateNode::Update> update_list = UpdateNode::Config::Instance()->updates();
        UpdateNode::Update update;

        if(update_list.size()>0)
            update = update_list.at(0);

        UpdateNode::Settings settings;
        QString cachedFile = settings.getCachedFile(update.getCode());

        if(update.getCode().isEmpty() || !QFile::exists(cachedFile))
        {
            qApp->exit(UPDATENODE_PROCERROR_RUN_DOWNLOAD_FIRST);
            return;
        }
        else
        {
            m_oReadyUpdates.append(update);
            install();
        }
    }
}

void SingleAppDialog::onDetailsCheck()
{
    adjustSize();
}

void SingleAppDialog::processError()
{
    m_pUi->chkDetails->show();
    m_pUi->textProgress->setTextColor(Qt::red);
    m_pUi->textProgress->append(m_oCommander.readStdErr());
    m_pUi->textProgress->show();
}

void SingleAppDialog::processOutput()
{
    m_pUi->chkDetails->show();
    m_pUi->textProgress->setTextColor(Qt::black);
    m_pUi->textProgress->append(m_oCommander.readStdOut());
    m_pUi->textProgress->show();
}

void SingleAppDialog::updateExit(int aExitCode, QProcess::ExitStatus aExitStatus)
{
    UpdateNode::Settings settings;

    settings.setUpdate(m_oCurrentUpdate, UpdateNode::LocalFile::getDownloadLocation(m_oCurrentUpdate.getDownloadLink()), aExitCode);

    if(aExitStatus == QProcess::NormalExit)
    {
        if(aExitCode == 0)
        {
            //m_pUI->labelProgress->setText(tr("Update '%1' installed successfully").arg(m_oCurrentUpdate.getTitle()));
            qDebug() << m_oCurrentUpdate.getTitle() << " updated successfully!";

            if(m_oCurrentUpdate.getTypeEnum() == UpdateNode::Update::CLIENT_SETS_VERSION)
                settings.setNewVersion(UpdateNode::Config::Instance(), UpdateNode::Config::Instance()->product(), m_oCurrentUpdate.getTargetVersion());

            accept();
        }
        else
        {
            //m_pUI->pshUpdate->show();
            //m_pUI->pshCheck->show();
            //m_pUI->labelProgress->setText(tr("Update '%1' failed with error %2").arg(m_oCurrentUpdate.getTitle()).arg(aExitCode));

            qDebug() << m_oCurrentUpdate.getTitle() << "updated failed - ErrorCode " << aExitCode;
            qApp->exit(UPDATENODE_PROCERROR_UPDATE_EXEC_FAILED);
        }
    }
    else
    {
        qDebug() << m_oCurrentUpdate.getTitle() << " crashed!";
        qApp->exit(UPDATENODE_PROCERROR_UPDATE_EXEC_CRASHED);
    }
}

void SingleAppDialog::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_pUi->progressBar->maximum() <= bytesTotal)
    {
        m_pUi->progressBar->setRange(0, bytesTotal);
        m_pUi->progressBar->setValue(bytesReceived);
    }
}

void SingleAppDialog::downloadDone(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString)
{
    m_oReadyUpdates.append(aUpdate);

    if(aError != QNetworkReply::NoError)
    {
        qDebug() << "Download failed: " << aErrorString;
        qApp->exit(UPDATENODE_PROCERROR_DOWNLOAD_FAILED);
        return;
    }

    if(!m_pDownloader->isDownloading())
        install();
}
