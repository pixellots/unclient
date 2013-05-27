#include "usermessages.h"
#include "ui_usermessages.h"
#include "config.h"

UserMessages::UserMessages(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::UserMessages)
{
    ui->setupUi(this);
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
    Sara::Config* config = Sara::Config::Instance();
    setWindowTitle(config->product().getName() + tr(" - Update Client"));

    if(!config->product().getIconUrl().isEmpty())
    {
        if(!config->mainIcon().isEmpty())
            setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        else
            setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    }
    else if(!config->mainIcon().isEmpty())
        setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));

    QList<Sara::Message> message_list= config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
       ui->label->setText(message_list.at(i).getTitle());
       if(!message_list.at(i).getMessage().isEmpty())
           ui->webView->setContent(message_list.at(i).getMessage().toUtf8());
       else
           ui->webView->load(message_list.at(i).getLink());
    }

    adjustSize();
    show();
}
