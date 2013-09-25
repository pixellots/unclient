/********************************************************************************
** Form generated from reading UI file 'usernotofication.ui'
**
** Created: Sun Sep 15 22:11:31 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERNOTOFICATION_H
#define UI_USERNOTOFICATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_UserNotofication
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *labelLogo;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelText;
    QLabel *labelDescription;
    QTreeWidget *treeWidget;
    QWebView *webView;
    QHBoxLayout *horizontalLayout;
    QPushButton *pshDetails;
    QSpacerItem *horizontalSpacer;
    QPushButton *pshNo;
    QPushButton *pshYes;

    void setupUi(QDialog *UserNotofication)
    {
        if (UserNotofication->objectName().isEmpty())
            UserNotofication->setObjectName(QString::fromUtf8("UserNotofication"));
        UserNotofication->resize(500, 500);
        gridLayout_2 = new QGridLayout(UserNotofication);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelLogo = new QLabel(UserNotofication);
        labelLogo->setObjectName(QString::fromUtf8("labelLogo"));

        gridLayout->addWidget(labelLogo, 0, 0, 3, 2);

        horizontalSpacer_2 = new QSpacerItem(11, 45, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 2, 1);

        labelText = new QLabel(UserNotofication);
        labelText->setObjectName(QString::fromUtf8("labelText"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelText->sizePolicy().hasHeightForWidth());
        labelText->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        labelText->setFont(font);

        gridLayout->addWidget(labelText, 0, 3, 1, 1);

        labelDescription = new QLabel(UserNotofication);
        labelDescription->setObjectName(QString::fromUtf8("labelDescription"));
        sizePolicy.setHeightForWidth(labelDescription->sizePolicy().hasHeightForWidth());
        labelDescription->setSizePolicy(sizePolicy);
        labelDescription->setWordWrap(true);
        labelDescription->setOpenExternalLinks(false);

        gridLayout->addWidget(labelDescription, 1, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        treeWidget = new QTreeWidget(UserNotofication);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy1);
        treeWidget->setMinimumSize(QSize(0, 125));
        treeWidget->setIconSize(QSize(24, 24));
        treeWidget->setRootIsDecorated(false);
        treeWidget->setUniformRowHeights(true);
        treeWidget->header()->setStretchLastSection(false);

        gridLayout_2->addWidget(treeWidget, 1, 0, 1, 1);

        webView = new QWebView(UserNotofication);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setMinimumSize(QSize(0, 250));
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        gridLayout_2->addWidget(webView, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pshDetails = new QPushButton(UserNotofication);
        pshDetails->setObjectName(QString::fromUtf8("pshDetails"));
        pshDetails->setAutoDefault(false);

        horizontalLayout->addWidget(pshDetails);

        horizontalSpacer = new QSpacerItem(168, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pshNo = new QPushButton(UserNotofication);
        pshNo->setObjectName(QString::fromUtf8("pshNo"));
        pshNo->setAutoDefault(false);

        horizontalLayout->addWidget(pshNo);

        pshYes = new QPushButton(UserNotofication);
        pshYes->setObjectName(QString::fromUtf8("pshYes"));
        pshYes->setDefault(true);

        horizontalLayout->addWidget(pshYes);


        gridLayout_2->addLayout(horizontalLayout, 3, 0, 1, 1);


        retranslateUi(UserNotofication);
        QObject::connect(pshYes, SIGNAL(clicked()), UserNotofication, SLOT(accept()));
        QObject::connect(pshNo, SIGNAL(clicked()), UserNotofication, SLOT(reject()));

        QMetaObject::connectSlotsByName(UserNotofication);
    } // setupUi

    void retranslateUi(QDialog *UserNotofication)
    {
        UserNotofication->setWindowTitle(QApplication::translate("UserNotofication", "Dialog", 0, QApplication::UnicodeUTF8));
        labelLogo->setText(QString());
        labelText->setText(QApplication::translate("UserNotofication", "Software updates are available", 0, QApplication::UnicodeUTF8));
        labelDescription->setText(QApplication::translate("UserNotofication", "Do you want to download and install them now?", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(4, QApplication::translate("UserNotofication", "Size", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(3, QApplication::translate("UserNotofication", "Version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("UserNotofication", "Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("UserNotofication", "Install", 0, QApplication::UnicodeUTF8));
        pshDetails->setText(QApplication::translate("UserNotofication", "Show Details", 0, QApplication::UnicodeUTF8));
        pshNo->setText(QApplication::translate("UserNotofication", "Not Now", 0, QApplication::UnicodeUTF8));
        pshYes->setText(QApplication::translate("UserNotofication", "Continue", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UserNotofication: public Ui_UserNotofication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERNOTOFICATION_H
