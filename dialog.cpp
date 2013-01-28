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
#include "downloader.h"
#include "settings.h"

Q_DECLARE_METATYPE ( Sara::Update );
Q_DECLARE_METATYPE ( Sara::Message);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_pUI(new Ui::DialogUpdate)
{
    m_pUI->setupUi(this);

    m_oTextEdit.hide();

    m_pProcess = new QProcess(this);
    connect(m_pProcess, SIGNAL(readyReadStandardError()), this, SLOT(processError()));
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(updateExit(int, QProcess::ExitStatus)));

    m_pDownloader = new Sara::Downloader();

    connect(m_pDownloader, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64, qint64)));
    connect(m_pDownloader, SIGNAL(done(const Sara::Update&)), SLOT(downloadDone(const Sara::Update&)));

    m_pUI->labelLogo->hide();
    m_pUI->labelProgress->hide();
    m_pUI->toolCancel->hide();
    m_pUI->progressBar->hide();

    connect(m_pUI->pshClose, SIGNAL(clicked()), SLOT(accept()));
    connect(m_pUI->pshCheck, SIGNAL(clicked()), SLOT(refresh()));
    connect(m_pUI->pshUpdate, SIGNAL(clicked()), SLOT(startInstall()));

    m_pUI->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    m_pUI->webViewMessage->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(m_pUI->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));
    connect(m_pUI->treeUpdate, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedUpdate()));
    connect(m_pUI->webViewMessage, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));
    connect(m_pUI->webViewMessage, SIGNAL(loadFinished(bool)), SLOT(messageLoaded(bool)));
    connect(m_pUI->treeMessage, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedMessage()));

    connect(m_pUI->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabSelected(int)));
}

Dialog::~Dialog()
{
    delete m_pUI;
}

void Dialog::init(Sara::Service* aService)
{
    m_pService = aService;

    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
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

        Sara::Update update = item->data(0, Qt::UserRole).value<Sara::Update>();
        m_pUI->webView->setContent(update.getDescription().toUtf8());

//        for(int i = 0; i < m_pUI->treeUpdate->topLevelItemCount(); i++)
//        {
//            QTreeWidgetItem* product = m_pUI->treeUpdate->topLevelItem(i);
//            for(int j = 0; j < product->childCount(); j++)
//                product->child(j)->setExpanded(FALSE);
//        }
//        item->setExpanded(TRUE);
    }
}

void Dialog::updateSelectedMessage()
{
    if(m_pUI->treeMessage->selectedItems().size()>0)
    {
        QTreeWidgetItem* item = m_pUI->treeMessage->selectedItems().at(0);

        Sara::Message message = item->data(0, Qt::UserRole).value<Sara::Message>();

        if(!message.getMessage().isEmpty())
            m_pUI->webViewMessage->setContent(message.getMessage().toUtf8());
        else
            m_pUI->webViewMessage->load(message.getLink());
    }
}

void Dialog::serviceDone()
{
    Sara::Config* config = Sara::Config::Instance();
    setWindowTitle(config->product().getName() + tr(" - Update Manager"));

    m_iNewMessages = 0;
    m_iNewUpdates = 0;
    m_pUI->treeUpdate->clear();
    m_pUI->treeMessage->clear();

    if(!config->product().getIconUrl().isEmpty())
    {
        m_pUI->labelLogo->setPixmap(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        m_pUI->labelLogo->show();
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    }
    else
        m_pUI->labelLogo->hide();

    m_pUI->labelVersion->setDisabled(TRUE);
    m_pUI->labelVersion->setText(tr("Current Version: %1").arg(config->version().getVersion()));

    updateUpdateView();
    updateMessageView();
    updateTabCounter();

    show();
}

void Dialog::updateUpdateView()
{
    Sara::Config* config = Sara::Config::Instance();

    QTreeWidgetItem* product= new QTreeWidgetItem(m_pUI->treeUpdate);
    product->setText(0, config->product().getName());

    QList<Sara::Update> update_list = config->updates();
    for(int i = 0; i < update_list.size(); i++)
    {
        QFont font;

        QTreeWidgetItem* parent = new QTreeWidgetItem(product);
        font.setBold(TRUE);
        parent->setFont(0, font);
        parent->setText(0, update_list.at(i).getTitle());
        parent->setCheckState(0, Qt::Checked);
        parent->setData(0, Qt::UserRole, QVariant::fromValue(update_list.at(i)));
        if(i==0)
            parent->setSelected(true);

        QTreeWidgetItem* sub = new QTreeWidgetItem(parent);
        sub->setText(0, tr("  Size: \t%1\n  Version: \t%2").arg(update_list.at(i).getFileSize()).arg(update_list.at(i).getTargetVersion().getVersion()));
        sub->setFlags(Qt::NoItemFlags);

        m_iNewUpdates++;
    }

    m_pUI->treeUpdate->expandAll();
}

void Dialog::updateMessageView()
{
    Sara::Settings settings;
    Sara::Config* config = Sara::Config::Instance();

    QTreeWidgetItem* product= new QTreeWidgetItem(m_pUI->treeMessage);
    product->setText(0, config->product().getName());

    QList<Sara::Message> message_list= config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
        QTreeWidgetItem* parent = new QTreeWidgetItem(product);

        if(!settings.messageShownAndLoaded(message_list.at(i).getCode()))
        {
            QFont font;
            font.setBold(TRUE);
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

void Dialog::updateTabCounter(bool aChangeTab /* = TRUE */)
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
        m_pUI->labelTitle->setText(tr("No Updates/Messages available on this computer"));

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
    Sara::Config::Instance()->clear();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_pUI->pshCheck->setDisabled(TRUE);

    m_pService->checkForUpdates();

    m_pUI->pshCheck->setDisabled(FALSE);
    QApplication::restoreOverrideCursor();
}

void Dialog::startInstall()
{
    m_pUI->labelProgress->show();
    m_pUI->toolCancel->show();
    m_pUI->progressBar->show();

    m_pUI->progressBar->setValue(0);
    
    m_oReadyUpdates.clear();

    QTreeWidgetItemIterator it(m_pUI->treeUpdate, QTreeWidgetItemIterator::Checked | QTreeWidgetItemIterator::Enabled);
    while (*it)
    {
        Sara::Update update = (*it)->data(0, Qt::UserRole).value<Sara::Update>();
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

void Dialog::downloadDone(const Sara::Update& aUpdate)
{
    m_oReadyUpdates.append(aUpdate);

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
        return;

    m_oTextEdit.clear();

    m_oCurrentUpdate = m_oReadyUpdates.takeFirst();

    QString command;
    QStringList commandParameters;
#ifdef Q_OS_LINUX // Linux
    if(m_oCurrentUpdate.isAdminRequired())
    {
        QString desktop = getenv("DESKTOP_SESSION");
        if(desktop.indexOf("kubuntu") != -1 || desktop.indexOf("kde") != -1)
            command = "/usr/bin/kdesudo";
        else
            command = "/usr/bin/gksudo";
    }
#else
#ifdef WIN32 // Windows

#else // Mac

#endif
#endif
    QString filename = QDir::tempPath() + QDir::separator() + "Sara" + QDir::separator() + QFileInfo(m_oCurrentUpdate.getDownloadLink()).fileName();
    QFile file(filename);
    file.setPermissions(QFile::ExeUser | QFile::ReadUser | QFile::WriteUser);

    if(command.isEmpty())
    {
        command = m_oCurrentUpdate.getCommand();
        if(!m_oCurrentUpdate.getCommandLine().isEmpty())
            commandParameters = QStringList() << m_oCurrentUpdate.getCommandLine().split(" ");
    }
    else
    {
        commandParameters << m_oCurrentUpdate.getCommand();
        if(!m_oCurrentUpdate.getCommandLine().isEmpty())
            commandParameters << m_oCurrentUpdate.getCommandLine().split(" ");
    }
    m_pUI->labelProgress->setText(tr("Installing Update '%1'").arg(m_oCurrentUpdate.getTitle()));

    qDebug() << "command: " << command;
    qDebug() << "commandlline: " << commandParameters.join(" ");

    m_pProcess->start(command, commandParameters);

    // wait 5 minutes for process start
    if(!m_pProcess->waitForStarted(1000 * 60 * 5))
    {
        m_pUI->labelProgress->setText(tr("Error: Update '%1' failed to start").arg(m_oCurrentUpdate.getTitle()));
        return;
    }
}

void Dialog::processError()
{
    m_oTextEdit.setTextColor(Qt::red);
    m_oTextEdit.append(m_pProcess->readAllStandardError());
    m_oTextEdit.show();
}

void Dialog::processOutput()
{
    m_oTextEdit.setTextColor(Qt::darkBlue);
    m_oTextEdit.append(m_pProcess->readAllStandardOutput());
    m_oTextEdit.show();
}

void Dialog::updateExit(int aExitCode, QProcess::ExitStatus aExitStatus)
{
    Sara::Settings settings;

    if(aExitStatus == QProcess::NormalExit)
    {
        if(aExitCode == 0)
        {
            m_pUI->labelProgress->setText(tr("Update '%1' installed successfully").arg(m_oCurrentUpdate.getTitle()));
            qDebug() << m_oCurrentUpdate.getTitle() << " updated successfully!";

            if(m_oCurrentUpdate.getTypeEnum() == Sara::Update::CLIENT_SETS_VERSION)
                settings.setNewVersion(Sara::Config::Instance()->product(), m_oCurrentUpdate.getTargetVersion());

            install();
        }
    }
    else
    {
        qDebug() << m_oCurrentUpdate.getTitle() << " carshed!";
    }

    settings.setUpdate(m_oCurrentUpdate, "TBD", aExitCode);
}

void Dialog::messageLoaded(bool aSuccess)
{
    Sara::Settings settings;

    QTreeWidgetItem* currentItem = m_pUI->treeMessage->selectedItems().at(0);

    if(currentItem && currentItem->parent())
    {
        Sara::Message message = currentItem->data(0, Qt::UserRole).value<Sara::Message>();

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
    font.setBold(FALSE);
    aItem->setFont(0, font);
    updateTabCounter(FALSE);
}

void Dialog::tabSelected(int aIndex)
{
    if(aIndex == 1)
    {
        Sara::Settings settings;
        QTreeWidgetItem* currentItem = m_pUI->treeMessage->selectedItems().at(0);

        if(currentItem && currentItem->parent())
        {
            Sara::Message message = currentItem->data(0, Qt::UserRole).value<Sara::Message>();

            if(!settings.messageShownAndLoaded(message.getCode()))
            {
                settings.setMessage(message, TRUE);
                if(settings.messageShownAndLoaded(message.getCode()))
                    resetMessageItem(currentItem);
            }
            else
                resetMessageItem(currentItem);
        }
    }
}


