#include "usernotofication.h"
#include "ui_usernotofication.h"
#include "config.h"
#include "settings.h"

#include <QDesktopServices>

Q_DECLARE_METATYPE ( Sara::Update );
Q_DECLARE_METATYPE ( Sara::Message);

UserNotofication::UserNotofication(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::UserNotofication)
{
    ui->setupUi(this);

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));

    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedUpdate()));
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(checkSelection()));

    connect(ui->pshDetails, SIGNAL(clicked()), SLOT(detailsClicked()));

    ui->treeWidget->hide();
    ui->webView->hide();

    ui->labelLogo->hide();

    adjustSize();
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

    setWindowTitle(config->product().getName() + tr(" - Update Client"));
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

    m_iNewUpdates = 0;
    m_iNewMessages = 0;

    if(config->updates().size()>0)
    {
       QList<Sara::Update> update_list = config->updates();
        for(int i = 0; i < update_list.size(); i++)
        {
            QTreeWidgetItem* parent = new QTreeWidgetItem(ui->treeWidget);
            parent->setData(0, Qt::UserRole, QVariant::fromValue(update_list.at(i)));
            parent->setCheckState(1, Qt::Checked);
            //parent->setIcon(0, QPixmap(":/images/updates.png"));
            parent->setText(2, update_list.at(i).getTitle());
            parent->setText(3, update_list.at(i).getTargetVersion().getVersion());
            parent->setText(4, update_list.at(i).getFileSize());
            if(i==0)
                parent->setSelected(true);

            m_iNewUpdates++;
        }
    }

    if(QTreeWidgetItem* header = ui->treeWidget->headerItem())
      header->setText(0, "");

    ui->treeWidget->header()->setResizeMode(2, QHeaderView::Stretch);
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(3);
    ui->treeWidget->resizeColumnToContents(4);
}

void UserNotofication::openLink(const QUrl& aUrl)
{
    QDesktopServices::openUrl(aUrl);
}

void UserNotofication::updateSelectedUpdate()
{
    if(ui->treeWidget->selectedItems().size()>0)
    {
        QTreeWidgetItem* item = ui->treeWidget->selectedItems().at(0);

        Sara::Update update = item->data(0, Qt::UserRole).value<Sara::Update>();
        ui->webView->setContent(update.getDescription().toUtf8());
    }
}

void UserNotofication::checkSelection()
{
    QTreeWidgetItemIterator it(ui->treeWidget);

    ui->pshYes->setEnabled(false);

    while(*it)
    {
        if((*it)->checkState(1) == Qt::Checked)
            ui->pshYes->setEnabled(true);
        it++;
    }
}

void UserNotofication::detailsClicked()
{
    if(ui->treeWidget->isVisible())
    {
        ui->treeWidget->hide();
        ui->webView->setHidden(true);
        ui->pshDetails->setText("Show Details");
        adjustSize();
    }
    else
    {
        ui->treeWidget->show();
        ui->webView->show();
        ui->pshDetails->setText("Hide Details");
    }
    layout()->setSizeConstraint(QLayout::SetMinimumSize);
}
