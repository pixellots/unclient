/********************************************************************************
** Form generated from reading UI file 'usermessages.ui'
**
** Created: Sun Sep 15 22:11:31 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMESSAGES_H
#define UI_USERMESSAGES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_UserMessages
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *labelLogo;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelText;
    QLabel *labelDescription;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolLeft;
    QLabel *label;
    QToolButton *toolRight;
    QWebView *webView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pshRead;
    QSpacerItem *horizontalSpacer;
    QProgressBar *progressBar;
    QPushButton *pshClose;

    void setupUi(QDialog *UserMessages)
    {
        if (UserMessages->objectName().isEmpty())
            UserMessages->setObjectName(QString::fromUtf8("UserMessages"));
        UserMessages->resize(500, 500);
        gridLayout_2 = new QGridLayout(UserMessages);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelLogo = new QLabel(UserMessages);
        labelLogo->setObjectName(QString::fromUtf8("labelLogo"));

        gridLayout->addWidget(labelLogo, 0, 0, 3, 2);

        horizontalSpacer_2 = new QSpacerItem(11, 45, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 2, 1);

        labelText = new QLabel(UserMessages);
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

        labelDescription = new QLabel(UserMessages);
        labelDescription->setObjectName(QString::fromUtf8("labelDescription"));
        sizePolicy.setHeightForWidth(labelDescription->sizePolicy().hasHeightForWidth());
        labelDescription->setSizePolicy(sizePolicy);
        labelDescription->setWordWrap(true);
        labelDescription->setOpenExternalLinks(false);

        gridLayout->addWidget(labelDescription, 1, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        toolLeft = new QToolButton(UserMessages);
        toolLeft->setObjectName(QString::fromUtf8("toolLeft"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/left.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolLeft->setIcon(icon);

        horizontalLayout->addWidget(toolLeft);

        label = new QLabel(UserMessages);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(0, 22));
        label->setFont(font);
        label->setFrameShape(QFrame::StyledPanel);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        toolRight = new QToolButton(UserMessages);
        toolRight->setObjectName(QString::fromUtf8("toolRight"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolRight->setIcon(icon1);

        horizontalLayout->addWidget(toolRight);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);

        webView = new QWebView(UserMessages);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        gridLayout_2->addWidget(webView, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pshRead = new QPushButton(UserMessages);
        pshRead->setObjectName(QString::fromUtf8("pshRead"));

        horizontalLayout_2->addWidget(pshRead);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        progressBar = new QProgressBar(UserMessages);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy2);
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(progressBar);

        pshClose = new QPushButton(UserMessages);
        pshClose->setObjectName(QString::fromUtf8("pshClose"));

        horizontalLayout_2->addWidget(pshClose);


        gridLayout_2->addLayout(horizontalLayout_2, 3, 0, 1, 1);


        retranslateUi(UserMessages);
        QObject::connect(pshClose, SIGNAL(clicked()), UserMessages, SLOT(accept()));
        QObject::connect(webView, SIGNAL(loadProgress(int)), progressBar, SLOT(setValue(int)));
        QObject::connect(webView, SIGNAL(loadFinished(bool)), progressBar, SLOT(setHidden(bool)));
        QObject::connect(webView, SIGNAL(loadStarted()), progressBar, SLOT(show()));
        QObject::connect(webView, SIGNAL(loadFinished(bool)), pshRead, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(UserMessages);
    } // setupUi

    void retranslateUi(QDialog *UserMessages)
    {
        UserMessages->setWindowTitle(QApplication::translate("UserMessages", "Dialog", 0, QApplication::UnicodeUTF8));
        labelLogo->setText(QString());
        labelText->setText(QApplication::translate("UserMessages", "Software messages are available", 0, QApplication::UnicodeUTF8));
        labelDescription->setText(QApplication::translate("UserMessages", "You can read them now, or later. Once you are done, mark the message as read. ", 0, QApplication::UnicodeUTF8));
        toolLeft->setText(QApplication::translate("UserMessages", "<", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("UserMessages", "TextLabel", 0, QApplication::UnicodeUTF8));
        toolRight->setText(QApplication::translate("UserMessages", ">", 0, QApplication::UnicodeUTF8));
        pshRead->setText(QApplication::translate("UserMessages", "Mark as read", 0, QApplication::UnicodeUTF8));
        pshClose->setText(QApplication::translate("UserMessages", "Not Now", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UserMessages: public Ui_UserMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMESSAGES_H
