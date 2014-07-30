/****************************************************************************
**
** Copyright (C) 2013 UpdatNode UG.
** Contact: code@updatenode.com
**
** This file is part of the UpdateNode Client.
**
** Commercial License Usage
** Licensees holding valid commercial UpdateNode license may use this file
** under the terms of the the Apache License, Version 2.0
** Full license description file: LICENSE.COM
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation. Please review the following information to ensure the
** GNU General Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
** Full license description file: LICENSE.GPL
**
****************************************************************************/

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QDateTime>

#include "commander.h"
#include "update.h"
#include "productversion.h"
#include "version.h"
#include "config.h"
#include "downloader.h"
#include "localfile.h"
#include "settings.h"
#include "security.h"
#include "updatenode_service.h"

class ClientTest : public QObject
{
    Q_OBJECT
    
public:
    ClientTest();

private Q_SLOTS:
    void test_security_hash();
    void test_security_signature();
    void test_version_compare();
    void test_commander_copy();
    void test_commander_resolve();
    void test_commander_split();
    void test_commander_run();
    void test_localfile_location();
    void test_settings_register();
    void test_settings_map();
    void test_downloader_download();
    void test_service_check();

private:
    UpdateNode::Update update;
    QString workingDir;
};


ClientTest::ClientTest()
{
    UpdateNode::ProductVersion target_version;

    update.setCode("code");
    update.setCommand("test");
    update.setCommandLine("");
    update.setDescription("description");
    update.setDownloadLink("http://updatenode.com/api/test.tst");
    update.setFileSize("1 MB");
    update.setRequiresAdmin(false);

    target_version.setCode("code_target");
    target_version.setName("name");
    target_version.setVersion("1.1");

    update.setTargetVersion(target_version);
    update.setTitle("title");
    update.setType(0);

    //UpdateNode::Config::Instance()->setLogging("-");
    UpdateNode::Config::Instance()->setKey("unittest");
    UpdateNode::Config::Instance()->setProductCode("product_code");
    UpdateNode::Config::Instance()->setVersion("1.0");
    UpdateNode::Config::Instance()->setSingleMode(true);

    // make sure we always have a clean working dir
    workingDir = QUuid::createUuid().toString();
    QDir::current().mkdir(workingDir);
    QDir::setCurrent(workingDir);
}

void ClientTest::test_commander_resolve()
{
    UpdateNode::Commander commander;

    commander.setUpdate(update);

    QVERIFY2(commander.resolve("[UN_UP_CODE]") == update.getCode(), "UN_UP_CODE");
    QVERIFY2(commander.resolve("[UN_UP_LINK]") == update.getDownloadLink(), "UN_UP_LINK");
    QVERIFY2(commander.resolve("[UN_UP_SIZE]") == update.getFileSize(), "UN_UP_SIZE");
    QVERIFY2(commander.resolve("[UN_UP_TARGETVERSION]") == update.getTargetVersion().getVersion(), "UN_UP_TARGETVERSION");
    QVERIFY2(commander.resolve("[UN_UP_TARGETCODE]") == update.getTargetVersion().getCode(), "UN_UP_TARGETCODE");
    QVERIFY2(commander.resolve("[UN_UP_TYPE]").toInt() == update.getType(), "UN_UP_TYPE");
    QVERIFY2(!commander.resolve("[UN_FILE]").isEmpty(), "UN_FILE");
    QVERIFY2(commander.resolve("[UN_FILENAME]") == "test.tst", "UN_FILENAME");
    QVERIFY2(commander.resolve("[UN_UP_LINK]") == update.getDownloadLink(), "UN_UP_LINK");

    update.setDownloadLink("updatenode.com/api/test.tst?lang=[UN_LANG]&[UN_VERSION]");
    QVERIFY2(update.getDownloadLink() == QString("http://updatenode.com/api/test.tst?lang=%1&%2").arg(QLocale::system().name()).arg("0.0"), qPrintable(update.getDownloadLink()));

}

void ClientTest::test_commander_copy()
{
    // remove possbile leftovers
    QFile::remove("test2");
    QFile::remove("../test2");

    QVERIFY2(UpdateNode::Commander::copy(qApp->applicationFilePath(), "test2"), "coyping the test binary to test2");
    QVERIFY2(QFile::exists("test2"), "Does the new file exist after copying?");
    int first_try = QFileInfo("test2").size();

    QVERIFY2(UpdateNode::Commander::copy("../test.pro", "test2"), "coyping again the another file to test2");
    QVERIFY2(QFile::exists("test2"), "Does the new file exist after copying?");
    int second_try = QFileInfo("test2").size();

    QVERIFY2(first_try != second_try, qPrintable(QString("Second try should recreate the file: %1").arg(first_try)));

    QVERIFY(QFile::remove("test2"));

    QVERIFY2(!UpdateNode::Commander::copy("test.negative", "test2"), "File does not exist");
    QVERIFY2(!UpdateNode::Commander::copy("", ""), "Empty files are not allowed");
    QVERIFY2(!UpdateNode::Commander::copy("..7test.pro", ""), "Empty files are not allowed");
    QVERIFY2(!UpdateNode::Commander::copy("", "test2"), "Empty files are not allowed");

    QVERIFY2(!UpdateNode::Commander::copy("../test.pro", ".."), "Copying does only work for files, not dirs");
    QVERIFY2(UpdateNode::Commander::copy("../test.pro", "../test2"), "Relative copying is allowed");
    QVERIFY2(UpdateNode::Commander::copy("../../unclient.rc", "test2"), "Relative copying is allowed");

    // clean up everything
    QVERIFY(QFile::remove("../test2"));
    QVERIFY(QFile::remove("test2"));
}

void ClientTest::test_commander_split()
{
    QStringList list;

    list = UpdateNode::Commander::splitCommandLine("test \"test test test\" test");

    QVERIFY2(list.size() == 3, qPrintable(QString::number(list.size())));
    QVERIFY2(list.at(0) == "test", qPrintable(list.at(0)));
    QVERIFY2(list.at(1) == "test test test", qPrintable(list.at(1)));
    QVERIFY2(list.at(2) == "test", qPrintable(list.at(2)));

    list = UpdateNode::Commander::splitCommandLine("-a -m\"test test\" -test *\"& test");

    QVERIFY2(list.size() == 6, qPrintable(QString::number(list.size())));
    QVERIFY2(list.at(0) == "-a", qPrintable(list.at(0)));
    QVERIFY2(list.at(1) == "-m", qPrintable(list.at(1)));
    QVERIFY2(list.at(2) == "test test", qPrintable(list.at(2)));
    QVERIFY2(list.at(3) == "-test", qPrintable(list.at(3)));
    QVERIFY2(list.at(4) == "*\"&", qPrintable(list.at(4)));
    QVERIFY2(list.at(5) == "test", qPrintable(list.at(5)));

    list = UpdateNode::Commander::splitCommandLine("-a -m\"test \"test\" test\"now_one_without_ending \"quote");

    QVERIFY2(list.size() == 7, qPrintable(QString::number(list.size())));
    QVERIFY2(list.at(0) == "-a", qPrintable(list.at(0)));
    QVERIFY2(list.at(1) == "-m", qPrintable(list.at(1)));
    QVERIFY2(list.at(2) == "test ", qPrintable(list.at(2)));
    QVERIFY2(list.at(3) == "test", qPrintable(list.at(3)));
    QVERIFY2(list.at(4) == " test", qPrintable(list.at(4)));
    QVERIFY2(list.at(5) == "now_one_without_ending", qPrintable(list.at(5)));
    QVERIFY2(list.at(6) == "\"quote", qPrintable(list.at(6)));
}

void ClientTest::test_commander_run()
{
    UpdateNode::Commander commander;

    UpdateNode::Update exec_update;
#ifndef Q_OS_MAC
    exec_update.setRequiresAdmin(false);
    exec_update.setCommand("dir");
    exec_update.setCommandLine("..");

    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    QVERIFY(commander.waitForFinished());
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
#endif
#ifndef Q_OS_WIN
    exec_update.setCommand("/bin/sh");
    exec_update.setCommandLine("-c\"echo this is a unit test only > test.log\"");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY2(QFile::remove("test.log"), qPrintable(commander.readStdOut()));

    exec_update.setCommand("/bin/sh");
    exec_update.setCommandLine("-c \"ls -l .. | grep cpp > test.log\"");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY(QFile::remove("test.log"));

    exec_update.setCommand("uname");
    exec_update.setCommandLine("-n -m");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY2(commander.readStdOut().split(" ").size()==2, qPrintable(commander.readStdOut()));

    exec_update.setCommand("/bin/sh");
    exec_update.setCommandLine("-c \"mkdir test_dir && cp ../*.* test_dir\"");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY(QDir("test_dir").exists() && QDir("test_dir").count()>3);
    exec_update.setCommand("rm");
    exec_update.setCommandLine("-fr test_dir");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
#endif
#ifdef Q_OS_WIN
    exec_update.setCommand("cmd.exe");
    exec_update.setCommandLine("/C \"echo this is a unit test only > test.log\"");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY2(QFile::remove("test.log"), qPrintable(commander.readStdOut()));

    exec_update.setCommand("ipconfig");
    exec_update.setCommandLine("/allcompartments /all");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY2(!commander.readStdOut().isEmpty(), qPrintable(commander.readStdOut()));

    exec_update.setCommand("cmd.exe");
    exec_update.setCommandLine("/C \"mkdir test_dir && copy ..\\*.* test_dir\"");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY(QDir("test_dir").exists() && QDir("test_dir").count()>3);
    exec_update.setCommand("cmd.exe");
    exec_update.setCommandLine("/C \"rmdir /S /Q test_dir\"");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
#endif
}

void ClientTest::test_version_compare()
{
    QVERIFY2(UpdateNode::Version::compare("1.0", "1.0.0") == 0, "equal");
    QVERIFY2(UpdateNode::Version::compare("1", "1") == 0, "equal");
    QVERIFY2(UpdateNode::Version::compare("1.0.0", "1.0.0") == 0, "equal");
    QVERIFY2(UpdateNode::Version::compare("1.0.0.0.0", "1.0.0") == 0, "equal");
    QVERIFY2(UpdateNode::Version::compare("1.0", "1") == 0, "equal");
    QVERIFY2(UpdateNode::Version::compare("", "") == 0, "equal");

    QVERIFY2(UpdateNode::Version::compare("2.0", "1.0.0") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2", "1") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0", "1.0.0") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0.0.0", "1.0.0") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0", "1") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.1", "2.0") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0.0.1", "2.0") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.1", "2.0") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.1.0.12321", "2.0.0.22222") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.1.12321", "2.0.0.22222") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0.33333", "2.0.0.22222") == -1, "first version is newer");
    QVERIFY2(UpdateNode::Version::compare("0.7", "0.1") == -1, "first version is newer");

    QVERIFY2(UpdateNode::Version::compare("2.0", "3.0.0") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2", "3") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0", "3.0.0") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0.0.0", "3.0.0") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0", "3") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.1", "3.0") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0.0.1", "3.0") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.1.0.12321", "2.2.0.22222") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.1.12321", "2.0.1.22222") == 1, "second version is newer");
    QVERIFY2(UpdateNode::Version::compare("2.0.0", "2.0.0.42222.0") == 1, "second version is newer");
}

void ClientTest::test_downloader_download()
{
    UpdateNode::Downloader downloader;
    QUrl url("http://updatenode.com/images/logo/updatenode_96.png");

    QNetworkReply* reply = downloader.doDownload(url, update);

    QVERIFY(reply != NULL);

    QEventLoop loop;
    QObject::connect(&downloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY(reply->error() == QNetworkReply::NoError);

    QVERIFY2(QFile::exists(UpdateNode::LocalFile::getDownloadLocation(url.toString())), qPrintable(UpdateNode::LocalFile::getDownloadLocation(url.toString())));

    reply = downloader.doDownload(url, update);
    QVERIFY(reply == NULL);

    QVERIFY(QFile::remove(UpdateNode::LocalFile::getDownloadLocation(url.toString())));

    // negative test
    url = url.fromUserInput("www.updatenode.com/fail");
    reply = downloader.doDownload(url, update);

    QVERIFY(reply != NULL);

    QObject::connect(&downloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY(reply->error() != QNetworkReply::NoError);

    QVERIFY2(!QFile::exists(UpdateNode::LocalFile::getDownloadLocation(url.toString())), qPrintable(UpdateNode::LocalFile::getDownloadLocation(url.toString())));
}

void ClientTest::test_localfile_location()
{
    QString tempPath = QDir::tempPath() + QDir::separator() + "UpdateNode" + QDir::separator() + UpdateNode::Config::Instance()->getKeyHashed();

    QVERIFY2(QDir(UpdateNode::LocalFile::getDownloadLocation("www.updatenode.com/hello.exe")) == QDir(tempPath + QDir::separator() + "hello.exe"), qPrintable(UpdateNode::LocalFile::getDownloadLocation("www.updatenode.com/hello.exe") + " NOT " + tempPath + QDir::separator() + "hello.exe"));
    QVERIFY(QDir(UpdateNode::LocalFile::getDownloadLocation("www.updatenode.com/hello")) == QDir(tempPath + QDir::separator() + "hello"));
    QVERIFY(QDir(UpdateNode::LocalFile::getDownloadLocation("http://www.updatenode.com/hello.world.exe")) == QDir(tempPath + QDir::separator() + "hello.world.exe"));
    QVERIFY(QDir(UpdateNode::LocalFile::getDownloadLocation("www.updatenode.com/hello.ini")) == QDir(tempPath + QDir::separator() + "hello.ini"));
    QVERIFY(QDir(UpdateNode::LocalFile::getDownloadLocation("www.updatenode.com")) == QDir(tempPath + QDir::separator() + "www.updatenode.com"));
    QVERIFY(QDir(UpdateNode::LocalFile::getDownloadLocation("")) == QDir(tempPath + QDir::separator()));
}

void ClientTest::test_service_check()
{
    UpdateNode::Service service;

    UpdateNode::Config::Instance()->setKey("demo");
    //UpdateNode::Config::Instance()->setTestKey("ef5dc6f7019738a18ee6b1573bcfbf5c");
    UpdateNode::Config::Instance()->setProductCode("unittest");
    UpdateNode::Config::Instance()->setVersion("3.0");

    UpdateNode::Config::Instance()->setSingleMode(true);
    UpdateNode::Config::Instance()->setSilent(true);

    QVERIFY(UpdateNode::Config::Instance()->isSingleMode());
    QEventLoop loop;
    QObject::connect(&service, SIGNAL(done()), &loop, SLOT(quit()));

    // version 3.0 has no updates and messages defined
    QVERIFY(service.checkForUpdates());
    loop.exec();
    QVERIFY(service.returnCode() == UpdateNode::Service::NOTHING);
    QVERIFY(service.status() == 0);

    // version 4.0 does not exists
    UpdateNode::Config::Instance()->setVersion("4.0");
    QVERIFY(service.checkForUpdates());
    loop.exec();
    QVERIFY(service.returnCode() == UpdateNode::Service::NOTHING);
    QVERIFY(service.status() == 1003);

    // version 1.0 has 1 update and 1 message
    UpdateNode::Config::Instance()->setVersion("1.0");
    QVERIFY(service.checkForUpdates());
    loop.exec();
    QVERIFY(service.returnCode() == UpdateNode::Service::UPDATE_MESSAGE);
    QVERIFY(service.status() == 0);
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTitle() == "unittest_update1");
    QVERIFY2(UpdateNode::Config::Instance()->updates().at(0).getDescription().toUtf8() == "<p>äüößÄÖÜ?!\"§$%&amp;/()=*'_:&lt;&gt;|^°`´+#-.@\\\\</p>", UpdateNode::Config::Instance()->updates().at(0).getDescription().toUtf8());
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getDownloadLink() == "http://updatenode.com/initial.data");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getFileSize() == "1 Test");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTargetVersion().getVersion() == "2.0");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTargetVersion().getCode() == "unittest_2");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTargetVersion().getName() == "unittest_2_name");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getType() == 2);
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getCommand() == "[UN_COPY_COMMAND]");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getCommandLine() == "[UN_FILE] test.unit");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).isAdminRequired() == false);
    QVERIFY(UpdateNode::Config::Instance()->messages().at(0).getTitle() == "unittest_message1");
    QVERIFY(UpdateNode::Config::Instance()->messages().at(0).getMessage().toUtf8() == "<p>unittest_message1</p>");
    QVERIFY(UpdateNode::Config::Instance()->messages().at(0).getLink().isEmpty());
    QVERIFY(UpdateNode::Config::Instance()->messages().at(0).isOpenExternal() == false);

    // download
    UpdateNode::Downloader downloader;
    QUrl url(UpdateNode::Config::Instance()->updates().at(0).getDownloadLink());
    QNetworkReply* reply = downloader.doDownload(url, UpdateNode::Config::Instance()->updates().at(0));
    if(reply != NULL)
    {
        QObject::connect(&downloader, SIGNAL(done(const UpdateNode::Update&, QNetworkReply::NetworkError, const QString&)), &loop, SLOT(quit()));
        loop.exec();
        QVERIFY(reply->error() == QNetworkReply::NoError);
    }
    // execute command
    UpdateNode::Commander commander;
    QVERIFY(commander.run(UpdateNode::Config::Instance()->updates().at(0)));
    commander.waitForFinished();

    commander.setUpdate(UpdateNode::Config::Instance()->updates().at(0));
    QVERIFY(QFile::exists(commander.resolve("test.unit")));

    QVERIFY(commander.getReturnCode()==0);

    // this is normally done via settings class for Type 2 updates
    UpdateNode::Config::Instance()->setVersion("2.0");

    // clean
    UpdateNode::Config::Instance()->clear();

    // check again
    QVERIFY(service.checkForUpdates());
    loop.exec();
    QVERIFY(service.returnCode() == UpdateNode::Service::UPDATE);
    QVERIFY(service.status() == 0);
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTitle() == "unittest_update2");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getDescription().toUtf8() == "<p>This is the second update</p>");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getDownloadLink() == "http://updatenode.com/initial.data");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getFileSize() == "2 Test");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTargetVersion().getVersion() == "3.0");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTargetVersion().getCode() == "unittest_3");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getTargetVersion().getName() == "unittest_3_name");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getType() == 1);
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getCommand() == "[UN_COPY_COMMAND]");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).getCommandLine() == "[UN_FILE] [UN_FILENAME].tmp");
    QVERIFY(UpdateNode::Config::Instance()->updates().at(0).isAdminRequired() == false);

    // execute command
    QVERIFY(commander.run(UpdateNode::Config::Instance()->updates().at(0)));
    commander.waitForFinished();

    commander.setUpdate(UpdateNode::Config::Instance()->updates().at(0));
    QVERIFY(QFile::exists(commander.resolve("[UN_FILENAME].tmp")));

    QVERIFY(commander.getReturnCode()==0);
}

void ClientTest::test_settings_register()
{
    UpdateNode::Settings settings;

    UpdateNode::Config::Instance()->setProductCode("unittest_register_test");
    UpdateNode::Config::Instance()->setVersion("1.0");
    UpdateNode::Config::Instance()->setSingleMode(true);

    QVERIFY(settings.registerVersion());
    UpdateNode::Config::Instance()->setProductCode("");
    UpdateNode::Config::Instance()->setVersion("");
    settings.getRegisteredVersion();
    QVERIFY(UpdateNode::Config::Instance()->configurations().size() == 1);
    for(int i = 0; i < UpdateNode::Config::Instance()->configurations().size(); i++)
    {
        QVERIFY(UpdateNode::Config::Instance()->configurations().at(i)->getProductCode() == "unittest_register_test");
        QVERIFY(UpdateNode::Config::Instance()->configurations().at(i)->getVersion() == "1.0");
    }
    UpdateNode::Config::Instance()->clear();

    UpdateNode::Config::Instance()->setProductCode("unittest_register_test");
    UpdateNode::Config::Instance()->setVersion("1.0");
    QVERIFY(settings.unRegisterVersion());
    settings.getRegisteredVersion();
    QVERIFY(UpdateNode::Config::Instance()->configurations().size() == 0);
}

void ClientTest::test_settings_map()
{
    UpdateNode::Settings settings;

    UpdateNode::Product product;
    UpdateNode::ProductVersion version;
    product.setCode("product_1234");
    product.setName("unittest_product");
    version.setCode("version_1234");
    version.setVersion("2.0");
    version.setName("unittest_version_name_2.0");

    UpdateNode::Config::Instance()->setProductCode("product_1234");
    UpdateNode::Config::Instance()->setVersion("1.0");
    UpdateNode::Config::Instance()->setSingleMode(true);

    settings.setNewVersion(UpdateNode::Config::Instance(), product, version);

    QVERIFY(settings.getProductCode()=="product_1234");
    QVERIFY(settings.getVersionCode()=="version_1234");
    QVERIFY(settings.getProductVersion()=="2.0");

    version.setCode("version_1234_2");
    version.setVersion("3.0");

    settings.setNewVersion(UpdateNode::Config::Instance(), product, version);

    QVERIFY(settings.getProductCode()=="product_1234");
    QVERIFY(settings.getProductVersion()=="3.0");
    QVERIFY(settings.getVersionCode()=="version_1234_2");

}

void ClientTest::test_security_hash()
{
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/left.png", "f9f80037efbb635b39ce1fea03b4071a693d7200"));
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/right.png", "4e76c3eb851586f6cb01ba415fa80f31c7a159f1"));
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/unclient.ico", "1719cbca21ad66831e3420973805c9a8da134963"));
    QVERIFY(!UpdateNode::Security::validateChecksum("../../images/unclient.ico", "invalid"));

    QVERIFY(UpdateNode::Security::validateChecksum("../../images/unclient.ico", "3957fcbb770e6aad7a3f2cf217bc75de", "md5"));
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/unclient.ico", "1719cbca21ad66831e3420973805c9a8da134963", "sha1"));
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/unclient.ico", "5dd5da9e7f4b8796323b21c90572e6c7705683f0deb065c82768d8f5", "sha224"));
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/unclient.ico", "931613cd59ec296cab7b31157275591cfda6509259d0de1103a8aa31df6bf369", "sha256"));
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/unclient.ico", "2ea77c3ad9240c0a9a8afc57a31a0efe5cbf0ffd1cb986b55438b6ef9d51337805ab46189fcda2962e41c3fd9caf403b", "sha384"));
    QVERIFY(UpdateNode::Security::validateChecksum("../../images/unclient.ico", "50bee95f1884e756e5d170424d686fcfb40a57d05eb43ba39508fb7d22fe97a89f60d9edc25f88a638be77687dfc2e89042686a47cbd01fc95a894288af72194", "sha512"));
}

void ClientTest::test_security_signature()
{
    QFile file("../../LICENSE.GPL");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QVERIFY(UpdateNode::Security::verfiySignature(file.readAll(), "b8d3d2b2b56ae2daf672c544de5729feb823a7abeb1a5deb16375ebb9c6d3d84c5230fae52ca5996114f3dde5efaceddee0535f65e3e6d24632b900a8dba9c0f1252e0cf8c704d06ef454a0692c391e5e5c249ec0cc6eccfa45c1a955ed537eb952bd286915266048caf43e97b71e708f445b2b045e28c74d0827f4d9a6e76bd0743e5cf0e994ea8df8db434865a9eeca49871729e0a215892d0b81090023c087974d87286b552ee8d6d72129a6ebfee8ec40f7b9ed7afbc24114655af85a474c51407f71984fa6ceb73b9586e7d651f7d0ae280887080ad831a64489f19bd6abc79918a4bf7f07307f62ddb527cd0974fd3681542b2d4785215f2503673d6dc", "../public.key"));
    QVERIFY(!UpdateNode::Security::verfiySignature(file.readAll(), "b8d3d2b2b56ae2daf672c544de5729feb823a7abeb1a5deb16375ebb9c6d3d84c5230fae52ca5996114f3dde5efaceddee0535f65e3e6d24632b900a8dba9c0f1252e0cf8c704d06ef454a0692c391e5e5c249ec0cc6eccfa45c1a955ed537eb952bd286915266048caf43e97b71e708f445b2b045e28c74d0827f4d9a6e76bd0743e5cf0e994ea8df8db434865a9eeca49871729e0a215892d0b81090023c087974d87286b552ee8d6d72129a6ebfee8ec40f7b9ed7afbc24114655af85a474c51407f71984fa6ceb73b9586e7d651f7d0ae280887080ad831a64489f19bd6abc79918a4bf7f07307f62ddb527cd0974fd3681542b2d4785215f2503673d6dc", "../public.key"));
    file.close();
}

QTEST_MAIN(ClientTest)

#include "tst_clienttest.moc"

