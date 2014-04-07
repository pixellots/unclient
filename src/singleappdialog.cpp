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

#include "singleappdialog.h"
#include "ui_singleappdialog.h"
#include "config.h"
#include "settings.h"
#include "product.h"
#include "localfile.h"
#include "usernotofication.h"
#include "status.h"
#include "logging.h"
#include "version.h"

/*!
\class SingleAppDialog
\brief Single application mode dialog, which shows only updates for one product
*/

SingleAppDialog::SingleAppDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    m_pUi(new Ui::SingleAppDialog),
    m_bDownloadOnly(false), m_bExecuteOnly(false)
{
    m_pUi->setupUi(this);

    m_pUi->textProgress->setHidden(true);

    connect(m_pUi->chkDetails, SIGNAL(stateChanged(int)), SLOT(onDetailsCheck()));
    connect(m_pUi->pushButton, SIGNAL(clicked()), SLOT(onCancel()));

    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processError()));
    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processOutput()));
    connect(&m_oCommander, SIGNAL(updateExit(int, QProcess::ExitStatus)), this, SLOT(updateExit(int, QProcess::ExitStatus)));

    m_pDownloader = new UpdateNode::Downloader();

    connect(m_pDownloader, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64, qint64)));
    connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), SLOT(downloadDone(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)));

    connect(this, SIGNAL(rejected()), SLOT(onClose()));
    connect(m_pUi->pushButton, SIGNAL(clicked()), SLOT(onClose()));

    m_pUi->chkDetails->hide();

    m_iErrorCode = UPDATENODE_PROCERROR_CANCELED;
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
            m_pUi->labelProgress->setText(tr("Downloading update ..."));
        else
            m_pUi->labelProgress->setText(tr("Downloading updates"));

        m_pDownloader->doDownload(update_list.at(i).getDownloadLink(), update_list.at(i));
    }
}

void SingleAppDialog::install()
{
    if(m_oReadyUpdates.isEmpty() || m_bDownloadOnly)
    {
        qApp->exit(UPDATENODE_PROCERROR_SUCCESS);
        return;
    }

    m_oCurrentUpdate = m_oReadyUpdates.takeFirst();

    m_pUi->labelProgress->setText(tr("Installing update ..."));
    m_pUi->progressBar->hide();
    m_pUi->pushButton->hide();
    hide();
    adjustSize();

    if(!m_oCommander.run(m_oCurrentUpdate))
    {
        qApp->exit(UPDATENODE_PROCERROR_COMMAND_LAUNCH_FAILED);
        return;
    }

    if(UpdateNode::Config::Instance()->isSilent())
       m_oCommander.waitForFinished();
}

void SingleAppDialog::serviceDone()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();
    setWindowTitle(config->product().getName() + tr(" - Update Client"));

    if(!config->mainIcon().isEmpty())
        setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    else if(!config->product().getIconUrl().isEmpty())
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));

    qApp->setWindowIcon(windowIcon());

    if(config->updates().size()==0)
    {
        qApp->exit(UPDATENODE_PROCERROR_NO_UPDATES);
        return;
    }

    if(!m_bExecuteOnly)
    {
        QList<UpdateNode::Update> update_list = config->updates();
        qSort(update_list.begin(), update_list.end(), UpdateNode::Version::toAscending);
        config->clear();
        config->addUpdate(update_list.at(0));

        if(!config->isSilent())
        {
            UserNotofication userNotify;
            userNotify.updateView();
            if(userNotify.exec() != QDialog::Accepted)
            {
                qApp->exit(UPDATENODE_PROCERROR_CANCELED);
                return;
            }
            userNotify.hide();
            adjustSize();
        }
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

        if(!config->isSilent())
        {
            adjustSize();
            show();
        }

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

void SingleAppDialog::onCancel()
{
    qApp->exit(m_iErrorCode);
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
    m_pUi->textProgress->setTextColor(Qt::blue);
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
            m_iErrorCode = UPDATENODE_PROCERROR_SUCCESS;

            m_pUi->labelProgress->setText(tr("Update '%1' installed successfully").arg(m_oCurrentUpdate.getTitle()));

            UpdateNode::Logging() << m_oCurrentUpdate.getTitle() << "updated successfully!";

            if(m_oCurrentUpdate.getTypeEnum() == UpdateNode::Update::CLIENT_SETS_VERSION)
                settings.setNewVersion(UpdateNode::Config::Instance(), UpdateNode::Config::Instance()->product(), m_oCurrentUpdate.getTargetVersion());

        }
        else
        {
            m_pUi->labelProgress->setText(tr("Update '%1' failed with error %2").arg(m_oCurrentUpdate.getTitle()).arg(aExitCode));

            UpdateNode::Logging() << m_oCurrentUpdate.getTitle() << "update failed - ErrorCode " << aExitCode;
            m_iErrorCode = UPDATENODE_PROCERROR_UPDATE_EXEC_FAILED;
        }
    }
    else
    {
        m_pUi->labelProgress->setText(tr("Update '%1' closed unexpected"));
        UpdateNode::Logging() << m_oCurrentUpdate.getTitle() << "crashed!";
        m_iErrorCode = UPDATENODE_PROCERROR_UPDATE_EXEC_CRASHED;
    }

    if(UpdateNode::Config::Instance()->isSilent() || m_oCurrentUpdate.getTypeEnum() == UpdateNode::Update::INSTALLER_SETS_VERSION)
        qApp->exit(m_iErrorCode);
    else
    {
        m_pUi->pushButton->show();
        m_pUi->pushButton->setText(QObject::tr("Close"));
        show();
    }
}

void SingleAppDialog::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(isHidden()
            && bytesReceived < (qint64)bytesTotal/3)
    {
        adjustSize();
        show();
    }

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
        UpdateNode::Logging() << "Download failed: " << aErrorString;
        qApp->exit(UPDATENODE_PROCERROR_DOWNLOAD_FAILED);
        return;
    }

    if(!m_pDownloader->isDownloading())
        install();
}

void SingleAppDialog::onClose()
{
    if(m_iErrorCode==-1)
        qApp->exit(UPDATENODE_PROCERROR_CANCELED);
}
