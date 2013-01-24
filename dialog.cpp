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

Q_DECLARE_METATYPE ( Sara::Update );
Q_DECLARE_METATYPE ( Sara::Message);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_pUI(new Ui::DialogUpdate)
{
    m_pUI->setupUi(this);

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
    connect(m_pUI->treeMessage, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedMessage()));
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

    m_pUI->treeUpdate->clear();

    if(!config->product().getIconUrl().isEmpty())
    {
        m_pUI->labelLogo->setPixmap(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        m_pUI->labelLogo->show();
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    }
    else
        m_pUI->labelLogo->hide();

    QString strMessage;

    if(config->updates().size()>0)
        strMessage = tr("Software Updates");
    else if(config->messages().size()>0)
        strMessage = tr("Product Messages");
    if(config->updates().size()>0 && config->messages().size()>0)
        strMessage = tr("Software Updates & Messages");

    if(!strMessage.isEmpty())
        m_pUI->labelTitle->setText(tr("%1 are available on this computer.").arg(strMessage));
    else
        m_pUI->labelTitle->setText(tr("No Updates/Messages available on this computer").arg(strMessage));

    updateUpdateView();
    updateMessageView();

    m_pUI->tabWidget->setCurrentIndex(0);

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
    }

    m_pUI->treeUpdate->expandAll();


    if(update_list.size()==0)
        m_pUI->tabWidget->setTabText(0, tr("Updates"));
    else
        m_pUI->tabWidget->setTabText(0, tr("Updates (%1)").arg(update_list.size()));
}

void Dialog::updateMessageView()
{
    Sara::Config* config = Sara::Config::Instance();

    QTreeWidgetItem* product= new QTreeWidgetItem(m_pUI->treeMessage);
    product->setText(0, config->product().getName());

    QList<Sara::Message> message_list= config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
        QFont font;

        QTreeWidgetItem* parent = new QTreeWidgetItem(product);
        font.setBold(TRUE);
        parent->setFont(0, font);
        parent->setText(0, message_list.at(i).getTitle());
        parent->setData(0, Qt::UserRole, QVariant::fromValue(message_list.at(i)));
        if(i==0)
            parent->setSelected(true);
    }

    m_pUI->treeMessage->expandAll();

    if(message_list.size()==0)
        m_pUI->tabWidget->setTabText(1, tr("Messages"));
    else
        m_pUI->tabWidget->setTabText(1, tr("Messages (%1)").arg(message_list.size()));
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
        m_pUI->labelProgress->setText(tr("Installing Updates ..."));

        install();
    }
}

void Dialog::install()
{
    QProcess* process = new QProcess(this);

    for(int i = 0; i < m_oReadyUpdates.size(); i++)
    {
        if(m_oReadyUpdates.at(i).isAdminRequired())
            qDebug() <<  "needs admin rights";
        QString filename = QDir::tempPath() + QDir::separator() + "Sara" + QDir::separator() + QFileInfo(m_oReadyUpdates.at(i).getDownloadLink()).fileName();
        QFile file(filename);
        file.setPermissions(QFile::ExeOwner);
        process->start(filename);
        process->waitForStarted(1000);
        qDebug() << process->errorString();
    }
}
