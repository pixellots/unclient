#include "singleappdialog.h"
#include "ui_singleappdialog.h"
#include "config.h"
#include "product.h"

SingleAppDialog::SingleAppDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    m_pUi(new Ui::SingleAppDialog)
{
    m_pUi->setupUi(this);

    m_pUi->textProgress->setHidden(true);

    connect(m_pUi->chkDetails, SIGNAL(stateChanged(int)), SLOT(onDetailsCheck()));
}

SingleAppDialog::~SingleAppDialog()
{
    delete m_pUi;
}

void SingleAppDialog::init(Sara::Service* aService)
{
    m_pService = aService;
    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
}

void SingleAppDialog::serviceDone()
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
    adjustSize();
    show();
}

void SingleAppDialog::onDetailsCheck()
{
    adjustSize();
}
