/********************************************************************************
** Form generated from reading UI file 'usermessages.ui'
**
** Created: Fri Oct 11 19:52:50 2013
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
    QWebView *webView;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolLeft;
    QLabel *label;
    QToolButton *toolRight;
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
        webView = new QWebView(UserMessages);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        gridLayout_2->addWidget(webView, 1, 0, 1, 1);

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
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 22));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        toolRight = new QToolButton(UserMessages);
        toolRight->setObjectName(QString::fromUtf8("toolRight"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolRight->setIcon(icon1);

        horizontalLayout->addWidget(toolRight);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pshRead = new QPushButton(UserMessages);
        pshRead->setObjectName(QString::fromUtf8("pshRead"));

        horizontalLayout_2->addWidget(pshRead);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        progressBar = new QProgressBar(UserMessages);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(progressBar);

        pshClose = new QPushButton(UserMessages);
        pshClose->setObjectName(QString::fromUtf8("pshClose"));

        horizontalLayout_2->addWidget(pshClose);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 0, 1, 1);


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
