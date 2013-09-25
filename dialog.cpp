#include <QFile>
#include <QtWebKit>
#include <QDesktopServices>

#include "dialog.h"
#include "ui_dialog.h"
#include "update.h"
#include "message.h"
#include "product.h"
#include "productversion.h"
#include "config.h"
#include "localfile.h"
#include "downloader.h"
#include "settings.h"

Q_DECLARE_METATYPE ( UpdateNode::Update );
Q_DECLARE_METATYPE ( UpdateNode::Message);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_pUI(new Ui::DialogUpdate)
{
    m_pUI->setupUi(this);

    m_oTextEdit.hide();

    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processError()));
    connect(&m_oCommander, SIGNAL(processError()), this, SLOT(processOutput()));
    connect(&m_oCommander, SIGNAL(updateExit(int, QProcess::ExitStatus)), this, SLOT(updateExit(int, QProcess::ExitStatus)));

    m_pDownloader = new UpdateNode::Downloader();

    connect(m_pDownloader, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64, qint64)));
    connect(m_pDownloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), SLOT(downloadDone(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)));

    m_pUI->labelLogo->hide();
    m_pUI->labelProgress->hide();
    m_pUI->toolCancel->hide();
    m_pUI->progressBar->hide();

    connect(m_pUI->pshClose, SIGNAL(clicked()), SLOT(accept()));
    connect(m_pUI->pshCheck, SIGNAL(clicked()), SLOT(refresh()));
    connect(m_pUI->pshUpdate, SIGNAL(clicked()), SLOT(startInstall()));
    connect(m_pUI->toolCancel, SIGNAL(clicked()), SLOT(cancelProgress()));

    m_pUI->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    m_pUI->webViewMessage->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(m_pUI->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));
    connect(m_pUI->treeUpdate, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedUpdate()));
    connect(m_pUI->treeUpdate, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(checkSelection()));
    connect(m_pUI->webViewMessage, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));
    connect(m_pUI->webViewMessage, SIGNAL(loadFinished(bool)), SLOT(messageLoaded(bool)));
    connect(m_pUI->treeMessage, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedMessage()));

    connect(m_pUI->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabSelected(int)));
}

Dialog::~Dialog()
{
    delete m_pUI;
}

void Dialog::init(UpdateNode::Service* aService)
{
    m_pService = aService;

    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
    connect(m_pService, SIGNAL(doneManager()), SLOT(serviceDoneManager()));
}

void Dialog::changeEvent(QEvent *e)
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

void Dialog::openLink(const QUrl& aUrl)
{
    QDesktopServices::openUrl(aUrl);
}

void Dialog::updateSelectedUpdate()
{
    if(m_pUI->treeUpdate->selectedItems().size()>0)
    {
        QTreeWidgetItem* item = m_pUI->treeUpdate->selectedItems().at(0);

        UpdateNode::Update update = item->data(0, Qt::UserRole).value<UpdateNode::Update>();
        m_pUI->webView->setContent(update.getDescription().toUtf8());
    }
}

void Dialog::checkSelection()
{
    QTreeWidgetItemIterator it(m_pUI->treeUpdate, QTreeWidgetItemIterator::Checked | QTreeWidgetItemIterator::Enabled);
    if(*it)
        m_pUI->pshUpdate->setEnabled(true);
    else
        m_pUI->pshUpdate->setEnabled(false);
}

void Dialog::updateSelectedMessage()
{
    if(m_pUI->treeMessage->selectedItems().size()>0)
    {
        QTreeWidgetItem* item = m_pUI->treeMessage->selectedItems().at(0);

        UpdateNode::Message message = item->data(0, Qt::UserRole).value<UpdateNode::Message>();

        if(!message.getMessage().isEmpty())
            m_pUI->webViewMessage->setContent(message.getMessage().toUtf8());
        else
            m_pUI->webViewMessage->load(message.getLink());
    }
}

void Dialog::serviceDone()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();
    
    setWindowTitle(config->product().getName() + tr(" - Update Manager"));

    m_iNewMessages = 0;
    m_iNewUpdates = 0;
    m_pUI->treeUpdate->clear();
    m_pUI->treeMessage->clear();
    m_pUI->webView->setContent("");
    m_pUI->webViewMessage->setContent("");

    if(!config->product().getIconUrl().isEmpty())
    {
        if(!config->mainIcon().isEmpty())
        {
            m_pUI->labelLogo->setPixmap(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
            m_pUI->labelLogo->show();
            setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        }
        else
        {
            m_pUI->labelLogo->hide();
            setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        }
    }
    else
        m_pUI->labelLogo->hide();

    m_pUI->labelVersion->show();
    m_pUI->labelVersion->setDisabled(true);
    m_pUI->labelVersion->setText(tr("Current Version: %1").arg(config->version().getVersion()));

    updateUpdateView();
    updateMessageView();
    updateTabCounter();

    m_pUI->pshUpdate->setFocus();
    show();
}

void Dialog::serviceDoneManager()
{
    UpdateNode::Config* globalConfig = UpdateNode::Config::Instance();

    m_iNewMessages = 0;
    m_iNewUpdates = 0;
    m_pUI->treeUpdate->clear();
    m_pUI->treeMessage->clear();
    m_pUI->webView->setContent("");
    m_pUI->webViewMessage->setContent("");

    if(!globalConfig->mainIcon().isEmpty())
    {
        m_pUI->labelLogo->setPixmap(QPixmap(globalConfig->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        m_pUI->labelLogo->show();
        setWindowIcon(QPixmap(globalConfig->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    }
    else
        m_pUI->labelLogo->hide();

    m_pUI->labelVersion->hide();
    m_pUI->pshCheck->hide();

    for(int i = 0; i < globalConfig->configurations().size();i++)
    {
        updateUpdateView(globalConfig->configurations().at(i));
        updateMessageView(globalConfig->configurations().at(i));
    }
    updateTabCounter();

    m_pUI->pshUpdate->setFocus();
    show();
}

void Dialog::cancelProgress()
{
    if(m_pDownloader->isDownloading())
        m_pDownloader->cancel();
}

void Dialog::updateUpdateView(UpdateNode::Config* aConfig /* = NULL */)
{
    UpdateNode::Config* config;

    if(aConfig)
        config = aConfig;
    else
        config = UpdateNode::Config::Instance();

    QFont font;
    font.setPointSize(qApp->font().pointSize()+1);

    QTreeWidgetItem* product= new QTreeWidgetItem(m_pUI->treeUpdate);
    product->setFont(0, font);
    product->setText(0, config->product().getName());
    product->setIcon(0, QPixmap(config->product().getLocalIcon()));

    QList<UpdateNode::Update> update_list = config->updates();
    for(int i = 0; i < update_list.size(); i++)
    {

        QTreeWidgetItem* parent = new QTreeWidgetItem(product);
        parent->setText(0, update_list.at(i).getTitle() + tr(" (Size: %1)").arg(update_list.at(i).getFileSize()));
        parent->setCheckState(0, Qt::Checked);
        parent->setData(0, Qt::UserRole, QVariant::fromValue(update_list.at(i)));
        if(i==0)
            parent->setSelected(true);

        m_iNewUpdates++;
    }

    product->setExpanded(true);

    m_pUI->pshUpdate->show();
    if(m_iNewUpdates==0)
        m_pUI->pshUpdate->setEnabled(false);
    else
        m_pUI->pshUpdate->setEnabled(true);

}

void Dialog::updateMessageView(UpdateNode::Config* aConfig /* = NULL */)
{
    UpdateNode::Settings settings;
    UpdateNode::Config* config;

    if(aConfig)
        config = aConfig;
    else
        config = UpdateNode::Config::Instance();

    QTreeWidgetItem* product= new QTreeWidgetItem(m_pUI->treeMessage);
    product->setText(0, config->product().getName());

    QList<UpdateNode::Message> message_list= config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
        QTreeWidgetItem* parent = new QTreeWidgetItem(product);

        if(!settings.messageShownAndLoaded(message_list.at(i).getCode()))
        {
            QFont font;
            font.setBold(true);
            parent->setFont(0, font);

            m_iNewMessages++;
        }
        parent->setText(0, message_list.at(i).getTitle());
        parent->setData(0, Qt::UserRole, QVariant::fromValue(message_list.at(i)));
        if(i==0)
            parent->setSelected(true);
    }

    m_pUI->treeMessage->expandAll();
}

void Dialog::updateTabCounter(bool aChangeTab /* = true */)
{
    if(aChangeTab)
    {
        if(m_iNewUpdates > 0)
            m_pUI->tabWidget->setCurrentIndex(0);
        else if(m_iNewMessages > 0)
            m_pUI->tabWidget->setCurrentIndex(1);
        else
            m_pUI->tabWidget->setCurrentIndex(0);
    }

    QString strMessage;

    if(m_iNewUpdates>0)
        strMessage = tr("Software Updates");
    else if(m_iNewMessages>0)
        strMessage = tr("Product Messages");
    if(m_iNewUpdates>0 && m_iNewMessages>0)
        strMessage = tr("Software Updates & Messages");

    if(!strMessage.isEmpty())
        m_pUI->labelTitle->setText(tr("%1 are available on this computer.").arg(strMessage));
    else
        m_pUI->labelTitle->setText(tr("No new Updates/Messages available on this computer"));

    if(m_iNewUpdates==0)
        m_pUI->tabWidget->setTabText(0, tr("Updates"));
    else
        m_pUI->tabWidget->setTabText(0, tr("Updates (%1)").arg(m_iNewUpdates));

    if(m_iNewMessages==0)
        m_pUI->tabWidget->setTabText(1, tr("Messages"));
    else
        m_pUI->tabWidget->setTabText(1, tr("Messages (%1)").arg(m_iNewMessages));
}

void Dialog::refresh()
{
    UpdateNode::Config::Instance()->clear();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_pUI->pshCheck->setDisabled(true);

    m_pService->checkForUpdates();

    m_pUI->pshCheck->setDisabled(false);
    QApplication::restoreOverrideCursor();
}

void Dialog::startInstall()
{
    m_pUI->labelProgress->show();
    m_pUI->toolCancel->show();
    m_pUI->progressBar->show();
    m_pUI->pshUpdate->hide();
    m_pUI->pshCheck->hide();

    m_pUI->progressBar->setValue(0);
    
    m_oReadyUpdates.clear();

    QTreeWidgetItemIterator it(m_pUI->treeUpdate, QTreeWidgetItemIterator::Checked | QTreeWidgetItemIterator::Enabled);
    while (*it)
    {
        UpdateNode::Update update = (*it)->data(0, Qt::UserRole).value<UpdateNode::Update>();
        m_pDownloader->doDownload(update.getDownloadLink(), update);
        m_pUI->labelProgress->setText("Downloading Updates...");
        ++it;
    }
}

void Dialog::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_pUI->progressBar->maximum() <= bytesTotal)
    {
        m_pUI->progressBar->setRange(0, bytesTotal);
        m_pUI->progressBar->setValue(bytesReceived);
    }
}

void Dialog::downloadDone(const UpdateNode::Update& aUpdate, QNetworkReply::NetworkError aError, const QString& aErrorString)
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

    if(!m_pDownloader->isDownloading())
    {
        m_pUI->progressBar->hide();
        m_pUI->toolCancel->hide();

        install();
    }
}

void Dialog::install()
{
    if(m_oReadyUpdates.size()==0)
    {
        m_pUI->pshCheck->show();
        return;
    }

    m_oTextEdit.clear();

    m_oCurrentUpdate = m_oReadyUpdates.takeFirst();

    if(!m_oCommander.run(m_oCurrentUpdate))
    {
        // TODO .... needs a check or somthing else
    }
}

void Dialog::processError()
{
    m_oTextEdit.setTextColor(Qt::red);
    m_oTextEdit.append(m_oCommander.readStdErr());
    m_oTextEdit.show();
}

void Dialog::processOutput()
{
    m_oTextEdit.setTextColor(Qt::darkBlue);
    m_oTextEdit.append(m_oCommander.readStdOut());
    m_oTextEdit.show();
}

void Dialog::updateExit(int aExitCode, QProcess::ExitStatus aExitStatus)
{
    UpdateNode::Settings settings;

    if(aExitStatus == QProcess::NormalExit)
    {
        if(aExitCode == 0)
        {
            m_pUI->labelProgress->setText(tr("Update '%1' installed successfully").arg(m_oCurrentUpdate.getTitle()));
            qDebug() << m_oCurrentUpdate.getTitle() << " updated successfully!";

            if(m_oCurrentUpdate.getTypeEnum() == UpdateNode::Update::CLIENT_SETS_VERSION)
                settings.setNewVersion(UpdateNode::Config::Instance()->product(), m_oCurrentUpdate.getTargetVersion());

            install();
        }
        else
        {
            m_pUI->pshUpdate->show();
            m_pUI->pshCheck->show();
            m_pUI->labelProgress->setText(tr("Update '%1' failed with error %2").arg(m_oCurrentUpdate.getTitle()).arg(aExitCode));

            qDebug() << m_oCurrentUpdate.getTitle() << " updated failed!";

        }
    }
    else
    {
        qDebug() << m_oCurrentUpdate.getTitle() << " crashed!";
    }

    settings.setUpdate(m_oCurrentUpdate, UpdateNode::LocalFile::getDownloadLocation(m_oCurrentUpdate.getDownloadLink()), aExitCode);
}

void Dialog::messageLoaded(bool aSuccess)
{
    UpdateNode::Settings settings;

    if(m_pUI->treeMessage->selectedItems().count()==0)
        return;

    QTreeWidgetItem* currentItem = m_pUI->treeMessage->selectedItems().at(0);

    if(currentItem && currentItem->parent())
    {
        UpdateNode::Message message = currentItem->data(0, Qt::UserRole).value<UpdateNode::Message>();

        if(!settings.messageShownAndLoaded(message.getCode()))
        {
            settings.setMessage(message, m_pUI->tabWidget->currentWidget() == m_pUI->tabMessage, aSuccess);
            if(settings.messageShownAndLoaded(message.getCode()))
                resetMessageItem(currentItem);
        }
        else
            resetMessageItem(currentItem);
    }
}

void Dialog::resetMessageItem(QTreeWidgetItem* aItem)
{
    QFont font = aItem->font(0);
    font.setBold(false);
    aItem->setFont(0, font);
    updateTabCounter(false);
}

void Dialog::tabSelected(int aIndex)
{
    if(aIndex == 1)
    {
        UpdateNode::Settings settings;
        QTreeWidgetItem* currentItem = m_pUI->treeMessage->selectedItems().at(0);

        if(currentItem && currentItem->parent())
        {
            UpdateNode::Message message = currentItem->data(0, Qt::UserRole).value<UpdateNode::Message>();

            if(!settings.messageShownAndLoaded(message.getCode()))
            {
                settings.setMessage(message, true);
                if(settings.messageShownAndLoaded(message.getCode()))
                    resetMessageItem(currentItem);
            }
            else
                resetMessageItem(currentItem);
        }
    }
}


