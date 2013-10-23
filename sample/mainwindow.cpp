#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "productdata.h"
#include "QProcess"
#include "QDir"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Test Product " + QString(MY_PRODUCT_VERSION));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::executeCommand(const QString& commandMode)
{
    QStringList commandParams;

    if(qApp->arguments().indexOf("-test")>-1)
        commandParams << "-t" << MY_UPDATENODE_TEST_KEY;

    commandParams << "-k"   << MY_UPDATENODE_KEY;
    commandParams << "-pc"  << MY_PRODUCT_CODE;
    commandParams << "-v"  << MY_PRODUCT_VERSION;
    commandParams << commandMode;

    return QProcess::execute(qApp->applicationDirPath() + QDir::separator() + "unclient", commandParams);
}

void MainWindow::on_actionCheck_for_updates_triggered()
{
    executeCommand("-update");
}

void MainWindow::on_actionCheck_for_messages_triggered()
{
    executeCommand("-messages");
}

void MainWindow::on_actionLaunch_Manager_triggered()
{
    executeCommand("-manager");
}
