#include "usernotofication.h"
#include "ui_usernotofication.h"
#include "config.h"
#include "settings.h"
#include "version.h"

#include <QDesktopServices>

Q_DECLARE_METATYPE ( UpdateNode::Update )
Q_DECLARE_METATYPE ( UpdateNode::Message)

UserNotofication::UserNotofication(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::UserNotofication)
{
    ui->setupUi(this);

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));

    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedUpdate()));
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

bool toAssending(UpdateNode::Update a, UpdateNode::Update b)
{
    return UpdateNode::Version::compare(a.getTargetVersion().getVersion(), b.getTargetVersion().getVersion()) == -1;
}

void UserNotofication::updateView()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();
    UpdateNode::Settings settings;

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

    if(config->updates().size()>0)
    {
        QList<UpdateNode::Update> update_list = config->updates();
        qSort(update_list.begin(), update_list.end(), toAssending);

        QTreeWidgetItem* parent = new QTreeWidgetItem(ui->treeWidget);
        parent->setData(0, Qt::UserRole, QVariant::fromValue(update_list.at(0)));
        parent->setText(0, update_list.at(0).getTitle());
        parent->setText(1, update_list.at(0).getTargetVersion().getVersion());
        parent->setText(2, update_list.at(0).getFileSize());
        parent->setSelected(true);
    }

    ui->treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->resizeColumnToContents(1);

    adjustSize();
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

        UpdateNode::Update update = item->data(0, Qt::UserRole).value<UpdateNode::Update>();
        ui->webView->setHtml(update.getDescription());
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
