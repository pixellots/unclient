/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Oct 11 19:54:09 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionClose_2;
    QAction *actionCheck_for_updates;
    QAction *actionCheck_for_messages;
    QAction *actionReview_updates_messages;
    QAction *actionAbout;
    QAction *actionGo_to_updatenode_com;
    QAction *actionGo_to_updateclient_com;
    QAction *actionLaunch_Manager;
    QAction *actionRefresh;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuUpdate;
    QMenu *menuAbout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(774, 497);
        actionClose_2 = new QAction(MainWindow);
        actionClose_2->setObjectName(QString::fromUtf8("actionClose_2"));
        actionCheck_for_updates = new QAction(MainWindow);
        actionCheck_for_updates->setObjectName(QString::fromUtf8("actionCheck_for_updates"));
        actionCheck_for_messages = new QAction(MainWindow);
        actionCheck_for_messages->setObjectName(QString::fromUtf8("actionCheck_for_messages"));
        actionReview_updates_messages = new QAction(MainWindow);
        actionReview_updates_messages->setObjectName(QString::fromUtf8("actionReview_updates_messages"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionGo_to_updatenode_com = new QAction(MainWindow);
        actionGo_to_updatenode_com->setObjectName(QString::fromUtf8("actionGo_to_updatenode_com"));
        actionGo_to_updateclient_com = new QAction(MainWindow);
        actionGo_to_updateclient_com->setObjectName(QString::fromUtf8("actionGo_to_updateclient_com"));
        actionLaunch_Manager = new QAction(MainWindow);
        actionLaunch_Manager->setObjectName(QString::fromUtf8("actionLaunch_Manager"));
        actionRefresh = new QAction(MainWindow);
        actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setReadOnly(false);

        gridLayout->addWidget(textBrowser, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 774, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuUpdate = new QMenu(menuBar);
        menuUpdate->setObjectName(QString::fromUtf8("menuUpdate"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuUpdate->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionClose_2);
        menuUpdate->addAction(actionCheck_for_updates);
        menuUpdate->addAction(actionCheck_for_messages);
        menuUpdate->addSeparator();
        menuUpdate->addAction(actionLaunch_Manager);
        menuUpdate->addSeparator();
        menuUpdate->addAction(actionRefresh);
        menuAbout->addAction(actionAbout);
        menuAbout->addSeparator();
        menuAbout->addAction(actionGo_to_updatenode_com);
        menuAbout->addAction(actionGo_to_updateclient_com);

        retranslateUi(MainWindow);
        QObject::connect(actionClose_2, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionClose_2->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionCheck_for_updates->setText(QApplication::translate("MainWindow", "Check for updates", 0, QApplication::UnicodeUTF8));
        actionCheck_for_messages->setText(QApplication::translate("MainWindow", "Check for messages", 0, QApplication::UnicodeUTF8));
        actionReview_updates_messages->setText(QApplication::translate("MainWindow", "Review updates & messages", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionGo_to_updatenode_com->setText(QApplication::translate("MainWindow", "Go to updatenode.com", 0, QApplication::UnicodeUTF8));
        actionGo_to_updateclient_com->setText(QApplication::translate("MainWindow", "Go to updateclient.com", 0, QApplication::UnicodeUTF8));
        actionLaunch_Manager->setText(QApplication::translate("MainWindow", "Launch Manager", 0, QApplication::UnicodeUTF8));
        actionRefresh->setText(QApplication::translate("MainWindow", "Refresh", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuUpdate->setTitle(QApplication::translate("MainWindow", "Update", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
