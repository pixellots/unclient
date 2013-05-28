#include "usermessages.h"
#include "ui_usermessages.h"
#include "config.h"
#include "settings.h"

#include <QDesktopServices>

UserMessages::UserMessages(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::UserMessages)
{
    ui->setupUi(this);

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));
    connect(ui->toolLeft, SIGNAL(clicked()), SLOT(onLeft()));
    connect(ui->toolRight, SIGNAL(clicked()), SLOT(onRight()));
    connect(ui->pshRead, SIGNAL(clicked()), SLOT(onRead()));

    ui->progressBar->hide();
}

UserMessages::~UserMessages()
{
    delete ui;
}

void UserMessages::init(Sara::Service* aService)
{
    m_pService = aService;
    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
}

void UserMessages::serviceDone()
{
    Sara::Settings settings;
    Sara::Config* config = Sara::Config::Instance();

    m_iCurrentIndex = -1;
    m_listMessages.clear();

    setWindowTitle(config->product().getName() + tr(" - Message Client"));
    if(!config->product().getIconUrl().isEmpty())
    {
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        if(!config->mainIcon().isEmpty())
            ui->labelLogo->setPixmap(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        else
            ui->labelLogo->setPixmap(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        ui->labelLogo->show();
    }
    else if(!config->mainIcon().isEmpty())
    {
        setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        ui->labelLogo->setPixmap(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        ui->labelLogo->show();
    }

    QList<Sara::Message> message_list= config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
        if(!settings.messageShownAndLoaded(message_list.at(i).getCode()))
            m_listMessages << message_list.at(i);
    }

    if(m_listMessages.isEmpty())
        qApp->quit();
    else
        show();

    layout()->setSizeConstraint(QLayout::SetMinimumSize);

    showMessage();
}


void UserMessages::showMessage()
{
    if(m_listMessages.isEmpty())
    {
        hide();
        qApp->quit();
        ui->label->setText(tr("No unread messages"));
        ui->webView->setContent("");
        ui->pshRead->setDisabled(true);
        ui->webView->hide();
        adjustSize();
        return;
    }

    if(m_iCurrentIndex == -1)
        m_iCurrentIndex = 0;

    ui->pshRead->setDisabled(true);

    if(m_listMessages.size()>1)
        ui->label->setText(m_listMessages.at(m_iCurrentIndex).getTitle() +  tr(" (%1/%2)").arg(m_iCurrentIndex+1).arg(m_listMessages.size()));
    else
        ui->label->setText(m_listMessages.at(m_iCurrentIndex).getTitle());

    if(!m_listMessages.at(m_iCurrentIndex).getMessage().isEmpty())
        ui->webView->setContent(m_listMessages.at(m_iCurrentIndex).getMessage().toUtf8());
    else
        ui->webView->load(m_listMessages.at(m_iCurrentIndex).getLink());

    ui->progressBar->setFormat(tr("Loading %p% ..."));

    if(m_listMessages.size()==m_iCurrentIndex+1)
        ui->toolRight->setDisabled(true);
    else
        ui->toolRight->setEnabled(true);

    if(m_iCurrentIndex == 0)
        ui->toolLeft->setDisabled(true);
    else
        ui->toolLeft->setEnabled(true);

    if(m_listMessages.size()==1)
    {
        ui->toolLeft->hide();
        ui->toolRight->hide();
        ui->pshRead->setText(tr("Mark as read and close"));
    }
}

void UserMessages::onLeft()
{
    m_iCurrentIndex--;
    showMessage();
}

void UserMessages::onRight()
{
    m_iCurrentIndex++;
    showMessage();
}

void UserMessages::onRead()
{
    Sara::Settings settings;
    Sara::Message message = m_listMessages.takeAt(m_iCurrentIndex);

    settings.setMessage(message, true, true);

    m_iCurrentIndex = -1;
    showMessage();
}

void UserMessages::openLink(const QUrl& aUrl)
{
    QDesktopServices::openUrl(aUrl);
}

