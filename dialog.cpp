#include <QFile>

#include "dialog.h"
#include "ui_dialog.h"
#include "update.h"
#include "message.h"
#include "product.h"
#include "productversion.h"
#include "config.h"
#include "downloader.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_pUI(new Ui::DialogUpdate)
{
    m_pUI->setupUi(this);

    m_pUI->labelDownloadSize->setText("");
    m_pUI->labelLogo->hide();

    connect(m_pUI->pshClose, SIGNAL(clicked()), SLOT(accept()));
    connect(m_pUI->pshCheck, SIGNAL(clicked()), SLOT(refresh()));
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

    QTreeWidgetItem* category= new QTreeWidgetItem(m_pUI->treeUpdate);
    category->setText(0, tr("Updates"));

    QList<Sara::Update> update_list = config->updates();
    for(int i = 0; i < update_list.size(); i++)
    {
        QFont font;

        QTreeWidgetItem* parent = new QTreeWidgetItem(category);
        font.setBold(TRUE);
        parent->setFont(0, font);
        parent->setText(0, update_list.at(i).getTitle());
        parent->setCheckState(0, Qt::Checked);

        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        font.setBold(FALSE);
        item->setFont(0, font);
        item->setText(0, update_list.at(i).getDescription());
    }

    category->setExpanded(TRUE);

    category= new QTreeWidgetItem(m_pUI->treeUpdate);
    category->setText(0, tr("Messages"));

    QList<Sara::Message> message_list = config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
        QFont font;

        QTreeWidgetItem* parent = new QTreeWidgetItem(category);
        font.setBold(TRUE);
        parent->setFont(0, font);
        parent->setText(0, message_list.at(i).getTitle());

        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        font.setBold(FALSE);
        item->setFont(0, font);
        item->setText(0, message_list.at(i).getMessage());

    }

    category->setExpanded(TRUE);

    show();
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
