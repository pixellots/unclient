/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Fri Oct 11 19:52:50 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_DialogUpdate
{
public:
    QGridLayout *gridLayout;
    QLabel *labelLogo;
    QPushButton *pshSettings;
    QSpacerItem *horizontalSpacer;
    QPushButton *pshClose;
    QTabWidget *tabWidget;
    QWidget *tabUpdate;
    QGridLayout *gridLayout_2;
    QTreeWidget *treeUpdate;
    QWebView *webView;
    QLabel *labelVersion;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pshCheck;
    QPushButton *pshUpdate;
    QProgressBar *progressBar;
    QToolButton *toolCancel;
    QLabel *labelProgress;
    QWidget *tabMessage;
    QGridLayout *gridLayout_3;
    QTreeWidget *treeMessage;
    QWebView *webViewMessage;
    QLabel *labelTitle;

    void setupUi(QDialog *DialogUpdate)
    {
        if (DialogUpdate->objectName().isEmpty())
            DialogUpdate->setObjectName(QString::fromUtf8("DialogUpdate"));
        DialogUpdate->resize(781, 635);
        gridLayout = new QGridLayout(DialogUpdate);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelLogo = new QLabel(DialogUpdate);
        labelLogo->setObjectName(QString::fromUtf8("labelLogo"));

        gridLayout->addWidget(labelLogo, 0, 0, 1, 1);

        pshSettings = new QPushButton(DialogUpdate);
        pshSettings->setObjectName(QString::fromUtf8("pshSettings"));
        pshSettings->setAutoDefault(false);

        gridLayout->addWidget(pshSettings, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(441, 26, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        pshClose = new QPushButton(DialogUpdate);
        pshClose->setObjectName(QString::fromUtf8("pshClose"));
        pshClose->setAutoDefault(false);

        gridLayout->addWidget(pshClose, 2, 3, 1, 1);

        tabWidget = new QTabWidget(DialogUpdate);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabUpdate = new QWidget();
        tabUpdate->setObjectName(QString::fromUtf8("tabUpdate"));
        gridLayout_2 = new QGridLayout(tabUpdate);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        treeUpdate = new QTreeWidget(tabUpdate);
        treeUpdate->setObjectName(QString::fromUtf8("treeUpdate"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeUpdate->sizePolicy().hasHeightForWidth());
        treeUpdate->setSizePolicy(sizePolicy);
        treeUpdate->setSelectionMode(QAbstractItemView::SingleSelection);
        treeUpdate->setIconSize(QSize(52, 52));
        treeUpdate->setRootIsDecorated(true);
        treeUpdate->setUniformRowHeights(false);
        treeUpdate->setAllColumnsShowFocus(true);
        treeUpdate->setWordWrap(true);
        treeUpdate->header()->setVisible(false);

        gridLayout_2->addWidget(treeUpdate, 0, 0, 1, 5);

        webView = new QWebView(tabUpdate);
        webView->setObjectName(QString::fromUtf8("webView"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(webView->sizePolicy().hasHeightForWidth());
        webView->setSizePolicy(sizePolicy1);
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        gridLayout_2->addWidget(webView, 1, 0, 1, 5);

        labelVersion = new QLabel(tabUpdate);
        labelVersion->setObjectName(QString::fromUtf8("labelVersion"));

        gridLayout_2->addWidget(labelVersion, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 2, 1, 1, 1);

        pshCheck = new QPushButton(tabUpdate);
        pshCheck->setObjectName(QString::fromUtf8("pshCheck"));
        pshCheck->setAutoDefault(false);

        gridLayout_2->addWidget(pshCheck, 2, 2, 1, 1);

        pshUpdate = new QPushButton(tabUpdate);
        pshUpdate->setObjectName(QString::fromUtf8("pshUpdate"));
        pshUpdate->setAutoDefault(true);
        pshUpdate->setDefault(true);

        gridLayout_2->addWidget(pshUpdate, 2, 3, 1, 2);

        progressBar = new QProgressBar(tabUpdate);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        gridLayout_2->addWidget(progressBar, 3, 0, 1, 4);

        toolCancel = new QToolButton(tabUpdate);
        toolCancel->setObjectName(QString::fromUtf8("toolCancel"));

        gridLayout_2->addWidget(toolCancel, 3, 4, 1, 1);

        labelProgress = new QLabel(tabUpdate);
        labelProgress->setObjectName(QString::fromUtf8("labelProgress"));

        gridLayout_2->addWidget(labelProgress, 4, 0, 1, 5);

        tabWidget->addTab(tabUpdate, QString());
        tabMessage = new QWidget();
        tabMessage->setObjectName(QString::fromUtf8("tabMessage"));
        gridLayout_3 = new QGridLayout(tabMessage);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        treeMessage = new QTreeWidget(tabMessage);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeMessage->setHeaderItem(__qtreewidgetitem);
        treeMessage->setObjectName(QString::fromUtf8("treeMessage"));
        sizePolicy.setHeightForWidth(treeMessage->sizePolicy().hasHeightForWidth());
        treeMessage->setSizePolicy(sizePolicy);
        treeMessage->header()->setVisible(false);

        gridLayout_3->addWidget(treeMessage, 0, 0, 1, 1);

        webViewMessage = new QWebView(tabMessage);
        webViewMessage->setObjectName(QString::fromUtf8("webViewMessage"));
        webViewMessage->setUrl(QUrl(QString::fromUtf8("about:blank")));

        gridLayout_3->addWidget(webViewMessage, 1, 0, 1, 1);

        tabWidget->addTab(tabMessage, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 4);

        labelTitle = new QLabel(DialogUpdate);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));

        gridLayout->addWidget(labelTitle, 0, 1, 1, 3);

        QWidget::setTabOrder(pshUpdate, toolCancel);
        QWidget::setTabOrder(toolCancel, pshSettings);
        QWidget::setTabOrder(pshSettings, pshClose);
        QWidget::setTabOrder(pshClose, tabWidget);
        QWidget::setTabOrder(tabWidget, treeUpdate);
        QWidget::setTabOrder(treeUpdate, webView);
        QWidget::setTabOrder(webView, pshCheck);
        QWidget::setTabOrder(pshCheck, treeMessage);
        QWidget::setTabOrder(treeMessage, webViewMessage);

        retranslateUi(DialogUpdate);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogUpdate);
    } // setupUi

    void retranslateUi(QDialog *DialogUpdate)
    {
        DialogUpdate->setWindowTitle(QApplication::translate("DialogUpdate", "Dialog", 0, QApplication::UnicodeUTF8));
        pshSettings->setText(QApplication::translate("DialogUpdate", "Settings", 0, QApplication::UnicodeUTF8));
        pshClose->setText(QApplication::translate("DialogUpdate", "Close", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeUpdate->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("DialogUpdate", "1", 0, QApplication::UnicodeUTF8));
        labelVersion->setText(QString());
        pshCheck->setText(QApplication::translate("DialogUpdate", "Check", 0, QApplication::UnicodeUTF8));
        pshUpdate->setText(QApplication::translate("DialogUpdate", "Install", 0, QApplication::UnicodeUTF8));
        toolCancel->setText(QApplication::translate("DialogUpdate", "x", 0, QApplication::UnicodeUTF8));
        labelProgress->setText(QApplication::translate("DialogUpdate", "TextLabel", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabUpdate), QApplication::translate("DialogUpdate", "&Updates", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabMessage), QApplication::translate("DialogUpdate", "&Messages", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogUpdate: public Ui_DialogUpdate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
