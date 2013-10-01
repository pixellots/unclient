#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QProcess>
#include <QProcess>
#include <QDir>
#include <QStatusBar>
#include "mainwindow.h"
#include "productdata.h"

QString getUpdateMessage(int code)
{
    QString text;
    switch(code)
    {
        case 0:
            text = QObject::tr("There are no new updates & messages available");
            break;
        case 1:
            text = QObject::tr("There is 1 update available");
            break;
        case 2:
            text = QObject::tr("There is 1 message available");
            break;
        case 3:
            text = QObject::tr("There are updates available");
            break;
        case 4:
            text = QObject::tr("There are messages available");
            break;
        case 5:
            text = QObject::tr("There is 1 update and 1 message available");
            break;
        case 6:
            text = QObject::tr("There is 1 update and messages available");
            break;
        case 7:
            text = QObject::tr("There are updates and 1 message available");
            break;
        case 8:
            text = QObject::tr("There are update and messages available");
            break;
        default:
            text = QObject::tr("Error %1 while checking for updates").arg(code);
            break;
    }
    return text;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen splash(QPixmap(":/images/images/splash_screen.png"));
    splash.showMessage("Checking for updates...", Qt::AlignCenter | Qt::AlignBottom);
    splash.show();

    QStringList commandParams;

    if(a.arguments().indexOf("-test")>-1)
        commandParams << "-t" << MY_UPDATENODE_TEST_KEY;

    commandParams << "-k"   << MY_UPDATENODE_KEY;
    commandParams << "-pc"  << MY_PRODUCT_CODE;
    commandParams << "-v"   << MY_PRODUCT_VERSION;

    commandParams << "-s";
    commandParams << "-check";

    int result = QProcess::execute(qApp->applicationDirPath() + QDir::separator() + "unclient", commandParams);

    MainWindow w;

    if(result!=0)
        w.statusBar()->showMessage(getUpdateMessage(result));
    else
        w.statusBar()->showMessage("Your product is up to date.");

    w.show();
    splash.finish(&w);
    
    return a.exec();
}
