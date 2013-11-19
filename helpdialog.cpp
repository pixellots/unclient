#include "helpdialog.h"
#include "ui_helpdialog.h"

Helpdialog::Helpdialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint),
    ui(new Ui::Helpdialog)
{
    ui->setupUi(this);

    setWindowIcon(QPixmap(":/images/updatenode.png"));
}

Helpdialog::~Helpdialog()
{
    delete ui;
}

void Helpdialog::setText(const QString& aAppName, const QString &aText)
{
    setWindowTitle(aAppName);

    ui->textEdit->setText(aAppName + "\n\n" + aText);
}
