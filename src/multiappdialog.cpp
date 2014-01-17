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

#include <QFile>
#include <QMessageBox>
#include <QDesktopServices>

#include "multiappdialog.h"
#include "ui_multiappdialog.h"
#include "update.h"
#include "message.h"
#include "product.h"
#include "productversion.h"
#include "config.h"
#include "localfile.h"
#include "downloader.h"
#include "settings.h"
#include "logging.h"
#include "version.h"

Q_DECLARE_METATYPE ( UpdateNode::Update )
Q_DECLARE_METATYPE ( UpdateNode::Config* )

/*!
\class MultiAppDialog
\brief Multi application mode dialog, which shows multiple registered products
*/

MultiAppDialog::MultiAppDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    m_pUI(new Ui::DialogUpdate)
{
    m_pUI->setupUi(this);

    m_iNewUpdates = 0;

    m_oTextEdit.hide();

    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processError()));
    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processOutput()));
    connect(&m_oCommander, SIGNAL(updateExit(int, QProcess::ExitStatus)), this, SLOT(updateExit(int, QProcess::ExitStatus)));

    m_pDownloader = new UpdateNode::Downloader();

    connect(m_pDownloader, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64, qint64)));
    connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), SLOT(downloadDone(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)));

    m_pUI->labelProgress->hide();
    m_pUI->toolCancel->hide();
    m_pUI->progressBar->hide();

    connect(m_pUI->pshClose, SIGNAL(clicked()), SLOT(accept()));
    connect(m_pUI->pshCheck, SIGNAL(clicked()), SLOT(refresh()));
    connect(m_pUI->pshUpdate, SIGNAL(clicked()), SLOT(startInstall()));
    connect(m_pUI->toolCancel, SIGNAL(clicked()), SLOT(cancelProgress()));

    connect(m_pUI->treeUpdate, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedUpdate()));
    connect(m_pUI->treeUpdate, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(checkSelection()));
    connect(m_pUI->treeUpdate, SIGNAL(customContextMenuRequested(QPoint)), SLOT(contextMenu(QPoint)));

    m_pUI->treeUpdate->setContextMenuPolicy(Qt::CustomContextMenu);
}

MultiAppDialog::~MultiAppDialog()
{
    delete m_pUI;
}

void MultiAppDialog::init(UpdateNode::Service* aService)
{
    m_pService = aService;

    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
    connect(m_pService, SIGNAL(doneManager()), SLOT(serviceDoneManager()));
}

void MultiAppDialog::initView()
{
    m_iNewUpdates = 0;
    m_pUI->treeUpdate->clear();
    m_pUI->textBrowser->setHtml("");

    QFont font;
    font.setPointSize(qApp->font().pointSize()+1);

    m_pIgnoredItem = new QTreeWidgetItem();
    font.setItalic(true);
    m_pIgnoredItem->setFont(0, font);
    m_pIgnoredItem->setTextColor(0, QColor::fromRgb(175,175,175));
    m_pIgnoredItem->setText(0, tr("Ignored"));
    m_pIgnoredItem->setFlags(Qt::ItemIsEnabled);
    m_pIgnoredItem->setHidden(true);
}

void MultiAppDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            m_pUI->retranslateUi(this);
            break;
        default:
             break;
    }
}

void MultiAppDialog::openLink(const QUrl& aUrl)
{
    QDesktopServices::openUrl(aUrl);
}

void MultiAppDialog::updateSelectedUpdate()
{
    if(m_pUI->treeUpdate->selectedItems().size()>0)
    {
        QTreeWidgetItem* item = m_pUI->treeUpdate->selectedItems().at(0);

        UpdateNode::Update update = item->data(0, Qt::UserRole).value<UpdateNode::Update>();
        m_pUI->textBrowser->setHtml(update.getDescription()     );
    }
}

void MultiAppDialog::checkSelection()
{
    QTreeWidgetItemIterator it(m_pUI->treeUpdate, QTreeWidgetItemIterator::Checked | QTreeWidgetItemIterator::Enabled);
    if(*it)
        m_pUI->pshUpdate->setEnabled(true);
    else
        m_pUI->pshUpdate->setEnabled(false);
}

#include <QMenu>
void MultiAppDialog::contextMenu(const QPoint& pos)
{
    QPoint globalPos = m_pUI->treeUpdate->mapToGlobal(pos);

    QMenu myMenu;

    if(!m_pUI->treeUpdate->currentItem()->parent())
        return;
    else if(m_pUI->treeUpdate->currentItem()->parent()->text(0) == tr("Ignored"))
        myMenu.addAction(tr("Don't ignore update"));
    else
        myMenu.addAction(tr("Ignore update"));

    QAction* selectedItem = myMenu.exec(globalPos);
    if(selectedItem)
    {
        UpdateNode::Settings settings;
        UpdateNode::Update update = m_pUI->treeUpdate->currentItem()->data(0, Qt::UserRole).value<UpdateNode::Update>();
        settings.setIgnoreUpdate(update.getCode(), selectedItem->text() == tr("Ignore update"));

        if(UpdateNode::Config::Instance()->isSingleMode())
            QTimer::singleShot(0, this, SLOT(serviceDone()));
        else
            QTimer::singleShot(0, this, SLOT(serviceDoneManager()));
    }
}

void MultiAppDialog::serviceDone()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();
    
    setWindowTitle(config->product().getName() + tr(" - Update Manager"));

    initView();

    if(!config->mainIcon().isEmpty())
        setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    else if(!config->product().getIconUrl().isEmpty())
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));

    qApp->setWindowIcon(windowIcon());

    updateView();
    updateCounter();

    m_pUI->pshUpdate->setFocus();
    checkSelection();

    if(!config->isSilent())
    {
        show();

        setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        raise();
        activateWindow();
    }
    else
        startInstall();
}

void MultiAppDialog::serviceDoneManager()
{
    UpdateNode::Config* globalConfig = UpdateNode::Config::Instance();

    setWindowTitle(tr("Software Update Manager"));

    initView();

    if(!globalConfig->mainIcon().isEmpty())
        setWindowIcon(QPixmap(globalConfig->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));

    qApp->setWindowIcon(windowIcon());

    m_pUI->pshCheck->hide();

    for(int i = 0; i < globalConfig->configurations().size();i++)
        updateView(globalConfig->configurations().at(i));

    updateCounter();

    m_pUI->pshUpdate->setFocus();
    checkSelection();

    if(!globalConfig->isSilent())
    {
        show();

        setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        raise();
        activateWindow();
    }
    else
        startInstall();
}

void MultiAppDialog::cancelProgress()
{
    if(m_pDownloader->isDownloading())
        m_pDownloader->cancel();
}

void MultiAppDialog::updateView(UpdateNode::Config* aConfig /* = NULL */)
{
    UpdateNode::Config* config;
    UpdateNode::Settings settings;

    if(aConfig)
        config = aConfig;
    else
        config = UpdateNode::Config::Instance();

    QFont font;
    font.setPointSize(qApp->font().pointSize()+1);

    if(config->updates().size()==0)
        return;

    QTreeWidgetItem* product= new QTreeWidgetItem(m_pUI->treeUpdate);
    product->setFont(0, font);
    product->setText(0, config->product().getName());
    product->setIcon(0, QPixmap(config->product().getLocalIcon()));
    product->setFlags(Qt::ItemIsEnabled);

    QList<UpdateNode::Update> update_list = config->updates();

    qSort(update_list.begin(), update_list.end(), UpdateNode::Version::toDescending);

    for(int i = 0; i < update_list.size(); i++)
    {
        QTreeWidgetItem* updateItem;

        if(settings.isUpdateIgnored(update_list.at(i).getCode()))
        {
            updateItem = new QTreeWidgetItem(m_pIgnoredItem);
            updateItem->setText(0, product->text(0) + ": " + update_list.at(i).getTitle() + tr(" (Size: %1)").arg(update_list.at(i).getFileSize()));
        }
        else
        {
            m_iNewUpdates++;
            updateItem = new QTreeWidgetItem(product);
            updateItem->setCheckState(0, Qt::Checked);
            updateItem->setText(0, update_list.at(i).getTitle() + tr(" (Size: %1)").arg(update_list.at(i).getFileSize()));
        }

        updateItem->setData(0, Qt::UserRole, QVariant::fromValue(update_list.at(i)));
        updateItem->setData(0, Qt::UserRole+1, QVariant::fromValue(config));

        if(m_pUI->treeUpdate->topLevelItemCount() == 1 && updateItem->parent()!=m_pIgnoredItem && product->childCount() == 1)
            updateItem->setSelected(true);
    }

    if(product->childCount()==0)
        delete product;
    else
        product->setExpanded(true);

    m_pUI->pshUpdate->show();
    if(m_iNewUpdates==0)
        m_pUI->pshUpdate->setEnabled(false);
    else
        m_pUI->pshUpdate->setEnabled(true);

}

void MultiAppDialog::updateCounter()
{
    QString strMessage;

    if(m_iNewUpdates==1)
        strMessage = tr("There is a new software update available");
    else if(m_iNewUpdates>1)
        strMessage = tr("There are %1 new software updates available").arg(m_iNewUpdates);
    else
        strMessage = tr("Your software is up to date");

    m_pUI->labelTitle->setText(strMessage);

    m_pUI->treeUpdate->addTopLevelItem(m_pIgnoredItem);
    m_pIgnoredItem->setHidden(m_pIgnoredItem->childCount()==0);
}

void MultiAppDialog::refresh()
{
    UpdateNode::Config::Instance()->clear();

    if(!UpdateNode::Config::Instance()->isSingleMode())
    {
        UpdateNode::Settings settings;
        settings.getRegisteredVersion();
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_pUI->pshCheck->setDisabled(true);

    m_pService->checkForUpdates();

    m_pUI->pshCheck->setDisabled(false);
    QApplication::restoreOverrideCursor();
}

void MultiAppDialog::startInstall()
{
    m_pUI->labelProgress->show();
    m_pUI->toolCancel->show();
    m_pUI->progressBar->show();
    m_pUI->pshUpdate->hide();
    m_pUI->pshCheck->hide();

    m_pUI->progressBar->setValue(0);
    
    m_oReadyUpdates.clear();
    m_bIsInstalling = false;

    m_pUI->treeUpdate->clearSelection();

    QTreeWidgetItemIterator it(m_pUI->treeUpdate, QTreeWidgetItemIterator::Checked | QTreeWidgetItemIterator::Enabled);
    if(*it)
    {
        UpdateNode::Logging() << (*it)->text(0);
        m_pCurrentItem = (*it);
        m_pCurrentItem->setSelected(true);
        UpdateNode::Update update = m_pCurrentItem->data(0, Qt::UserRole).value<UpdateNode::Update>();
        m_pUI->labelProgress->setText(tr("Downloading update %1 ...").arg(update.getTitle()));
        m_pDownloader->doDownload(update.getDownloadLink(), update);
        return;
    }

    qApp->processEvents();

    m_pUI->toolCancel->hide();
    m_pUI->progressBar->hide();
    m_pUI->pshCheck->show();
    m_pUI->labelProgress->setText(tr("All updates have been installed successfully"));
    qApp->processEvents();
}

void MultiAppDialog::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_pUI->progressBar->maximum() <= bytesTotal)
    {
        m_pUI->progressBar->setRange(0, bytesTotal);
        m_pUI->progressBar->setValue(bytesReceived);
    }
}

void MultiAppDialog::downloadDone(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString)
{
    m_oReadyUpdates.append(aUpdate);

    if(aError != QNetworkReply::NoError)
    {
        m_pUI->progressBar->hide();
        m_pUI->toolCancel->hide();
        m_pUI->pshUpdate->show();
        m_pUI->pshCheck->show();
        m_pUI->labelProgress->setText(aErrorString);
        return;
    }

    m_pUI->progressBar->hide();
    m_pUI->toolCancel->hide();

    if(!m_bIsInstalling)
        install();
}

void MultiAppDialog::install()
{
    m_bIsInstalling = true;

    m_oTextEdit.clear();

    m_oCurrentUpdate = m_oReadyUpdates.takeFirst();

    m_pUI->labelProgress->setText(tr("Installing update \"%1\"").arg(m_oCurrentUpdate.getTitle()));

    if(!m_oCommander.run(m_oCurrentUpdate))
    {
        m_pCurrentItem->setTextColor(0, QColor("red"));

        QMessageBox::critical(this, m_oCurrentUpdate.getTitle(), tr("Update failed:<br>%1").arg(tr("Unable to execute the command!")));

        m_pCurrentItem->setCheckState(0, Qt::Unchecked);
        m_pCurrentItem->setFlags(Qt::NoItemFlags);
        m_bIsInstalling = false;
        startInstall();
    }
}

void MultiAppDialog::processError()
{
    m_oTextEdit.setTextColor(Qt::red);
    m_oTextEdit.append(m_oCommander.readStdErr());
    m_oTextEdit.show();
}

void MultiAppDialog::processOutput()
{
    m_oTextEdit.setTextColor(Qt::darkBlue);
    m_oTextEdit.append(m_oCommander.readStdOut());
    m_oTextEdit.show();
}

void MultiAppDialog::updateExit(int aExitCode, QProcess::ExitStatus aExitStatus)
{
    UpdateNode::Settings settings;

    if(aExitStatus == QProcess::NormalExit)
    {
        if(aExitCode == 0)
        {
            m_pUI->labelProgress->setText(tr("Update '%1' installed successfully").arg(m_oCurrentUpdate.getTitle()));
            UpdateNode::Logging() << m_oCurrentUpdate.getTitle() << " updated successfully!";

            if(m_oCurrentUpdate.getTypeEnum() == UpdateNode::Update::CLIENT_SETS_VERSION)
            {
                if(UpdateNode::Config::Instance()->isSingleMode())
                    settings.setNewVersion(UpdateNode::Config::Instance(), UpdateNode::Config::Instance()->product(), m_oCurrentUpdate.getTargetVersion());
                else
                {
                    UpdateNode::Config* config = m_pCurrentItem->data(0, Qt::UserRole+1).value<UpdateNode::Config*>();
                    settings.setNewVersion(config, config->product(), m_oCurrentUpdate.getTargetVersion());
                }
            }

            m_pCurrentItem->setTextColor(0, QColor("green"));
        }
        else
        {
            m_pUI->pshUpdate->show();
            m_pUI->pshCheck->show();
            m_pUI->labelProgress->setText(tr("Update '%1' failed with error %2").arg(m_oCurrentUpdate.getTitle()).arg(aExitCode));

            UpdateNode::Logging() << m_oCurrentUpdate.getTitle() << "updated failed - ErrorCode " << aExitCode;
            m_pCurrentItem->setTextColor(0, QColor("red"));
        }
    }
    else
    {
        UpdateNode::Logging() << m_oCurrentUpdate.getTitle() << " crashed!";
        m_pCurrentItem->setTextColor(0, QColor("red"));
    }

    settings.setUpdate(m_oCurrentUpdate, UpdateNode::LocalFile::getDownloadLocation(m_oCurrentUpdate.getDownloadLink()), aExitCode);

    m_pCurrentItem->setCheckState(0, Qt::Unchecked);
    m_pCurrentItem->setFlags(Qt::NoItemFlags);
    m_bIsInstalling = false;
    startInstall();
}
