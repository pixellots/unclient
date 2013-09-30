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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCheck_for_updates_triggered()
{
    QStringList commandParams;

    if(qApp->arguments().indexOf("-test")>-1)
        commandParams << "-t" << MY_UPDATENODE_KEY;

    commandParams << "-k"   << MY_UPDATENODE_KEY;
    commandParams << "-pc"  << MY_PRODUCT_CODE;
    commandParams << "-v"  << MY_PRODUCT_VERSION;
    commandParams << "-updates";

    QProcess::execute(qApp->applicationDirPath() + QDir::separator() + "unclient", commandParams);
}
