/********************************************************************************
** Form generated from reading UI file 'singleappdialog.ui'
**
** Created: Fri Oct 11 19:52:50 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SINGLEAPPDIALOG_H
#define UI_SINGLEAPPDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SingleAppDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelProgress;
    QProgressBar *progressBar;
    QTextEdit *textProgress;
    QHBoxLayout *horizontalLayout;
    QCheckBox *chkDetails;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SingleAppDialog)
    {
        if (SingleAppDialog->objectName().isEmpty())
            SingleAppDialog->setObjectName(QString::fromUtf8("SingleAppDialog"));
        SingleAppDialog->resize(420, 199);
        gridLayout = new QGridLayout(SingleAppDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelProgress = new QLabel(SingleAppDialog);
        labelProgress->setObjectName(QString::fromUtf8("labelProgress"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelProgress->sizePolicy().hasHeightForWidth());
        labelProgress->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(labelProgress);

        progressBar = new QProgressBar(SingleAppDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(400, 0));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(progressBar);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        textProgress = new QTextEdit(SingleAppDialog);
        textProgress->setObjectName(QString::fromUtf8("textProgress"));

        gridLayout->addWidget(textProgress, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        chkDetails = new QCheckBox(SingleAppDialog);
        chkDetails->setObjectName(QString::fromUtf8("chkDetails"));

        horizontalLayout->addWidget(chkDetails);

        buttonBox = new QDialogButtonBox(SingleAppDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

        horizontalLayout->addWidget(buttonBox);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(SingleAppDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SingleAppDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SingleAppDialog, SLOT(reject()));
        QObject::connect(chkDetails, SIGNAL(toggled(bool)), textProgress, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(SingleAppDialog);
    } // setupUi

    void retranslateUi(QDialog *SingleAppDialog)
    {
        SingleAppDialog->setWindowTitle(QApplication::translate("SingleAppDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        labelProgress->setText(QApplication::translate("SingleAppDialog", "Initalizing ...", 0, QApplication::UnicodeUTF8));
        chkDetails->setText(QApplication::translate("SingleAppDialog", "Show details", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SingleAppDialog: public Ui_SingleAppDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SINGLEAPPDIALOG_H
