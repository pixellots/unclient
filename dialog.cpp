#include "dialog.h"
#include "ui_dialog.h"
#include "update.h"
#include "message.h"
#include "product.h"
#include "productversion.h"
#include "config.h"

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

    QList<Sara::Update> list = config->updates();
    for(int i = 0; i < list.size(); i++)
    {
        QFont font;
        QTreeWidgetItem* parent = new QTreeWidgetItem(m_pUI->treeUpdate);

        font.setBold(TRUE);
        parent->setFont(0, font);
        parent->setText(0, list.at(i).getTitle());
        parent->setCheckState(0, Qt::Checked);

        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        font.setBold(FALSE);
        item->setFont(0, font);
        item->setText(0, list.at(i).getDescription());

        parent->setExpanded(TRUE);
    }

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
