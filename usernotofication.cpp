#include "usernotofication.h"
#include "ui_usernotofication.h"
#include "config.h"
#include "settings.h"

Q_DECLARE_METATYPE ( Sara::Update );
Q_DECLARE_METATYPE ( Sara::Message);

UserNotofication::UserNotofication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserNotofication)
{
    ui->setupUi(this);
}

UserNotofication::~UserNotofication()
{
    delete ui;
}

void UserNotofication::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UserNotofication::updateView()
{
    Sara::Config* config = Sara::Config::Instance();
    Sara::Settings settings;

    m_iNewUpdates = 0;
    m_iNewMessages = 0;

    QFont font;
    font.setPointSize(qApp->font().pointSize()+1);
    font.setBold(true);

    if(config->messages().size()>0)
    {
        QTreeWidgetItem* messages = new QTreeWidgetItem(ui->treeWidget);
        messages->setIcon(0, QPixmap(":/images/messages.png"));
        messages->setText(0, tr("Messages"));
        messages->setFont(0, font);

        QList<Sara::Message> message_list= config->messages();
        for(int i = 0; i < message_list.size(); i++)
        {
            if(!settings.messageShownAndLoaded(message_list.at(i).getCode()))
            {
                QTreeWidgetItem* parent = new QTreeWidgetItem(messages);

                m_iNewMessages++;

                parent->setText(0, message_list.at(i).getTitle());
                parent->setData(0, Qt::UserRole, QVariant::fromValue(message_list.at(i)));

            }
         }
    }

    if(config->updates().size()>0)
    {
        QTreeWidgetItem* updates = new QTreeWidgetItem(ui->treeWidget);
        updates->setFont(0, font);
        updates->setText(0, tr("Updates"));
        updates->setIcon(0, QPixmap(":/images/updates.png"));

        QList<Sara::Update> update_list = config->updates();
        for(int i = 0; i < update_list.size(); i++)
        {
            QTreeWidgetItem* parent = new QTreeWidgetItem(updates);
            parent->setText(0, update_list.at(i).getTitle() + tr(" (Size: %1)").arg(update_list.at(i).getFileSize()));
            parent->setData(0, Qt::UserRole, QVariant::fromValue(update_list.at(i)));
            parent->setCheckState(0, Qt::Checked);
            if(i==0)
                parent->setSelected(true);

            m_iNewUpdates++;
        }
    }

    ui->treeWidget->expandAll();

    adjustSize();
}
