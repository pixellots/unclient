#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QDateTime>

#include "../commander.h"
#include "../update.h"
#include "../productversion.h"
#include "../version.h"
#include "../config.h"
#include "../downloader.h"

class ClientTest : public QObject
{
    Q_OBJECT
    
public:
    ClientTest();
    
private Q_SLOTS:
    void test_version_compare();
    void test_commander_copy();
    void test_commander_resolve();
    void test_commander_split();
    void test_commander_run();
    void test_downloader_download();

private:
    UpdateNode::Update update;
};


ClientTest::ClientTest()
{
    UpdateNode::ProductVersion target_version;

    update.setCode("code");
    update.setCommand("test");
    update.setCommandLine("");
    update.setDescription("description");
    update.setDownloadLink("http://updatenode.com/test/test.tst");
    update.setFileSize("1 MB");
    update.setRequiresAdmin(false);

    target_version.setCode("code_target");
    target_version.setName("name");
    target_version.setVersion("1.1");

    update.setTargetVersion(target_version);
    update.setTitle("title");
    update.setType(0);

    UpdateNode::Config::Instance()->setLogging("-");
    UpdateNode::Config::Instance()->setKey("unittest");
    UpdateNode::Config::Instance()->setProductCode("product_code");
    UpdateNode::Config::Instance()->setVersion("version");
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

    update.setDownloadLink("updatenode.com/test/test.tst?lang=[UN_LANG]&[UN_VERSION]");
    QVERIFY2(update.getDownloadLink() == QString("http://updatenode.com/test/test.tst?lang=%1&%2").arg(QLocale::system().name()).arg(UPDATENODE_CLIENT_VERSION), qPrintable(update.getDownloadLink()));

}

void ClientTest::test_commander_copy()
{
    // remove possbile leftovers
    QFile::remove("test2");
    QFile::remove("../test2");

    QVERIFY2(UpdateNode::Commander::copy(qApp->applicationFilePath(), "test2"), "coyping the test binary to test2");
    QVERIFY2(QFile::exists("test2"), "Does the new file exist after copying?");
    int first_try = QFileInfo("test2").size();

    QVERIFY2(UpdateNode::Commander::copy("test.pro", "test2"), "coyping again the another file to test2");
    QVERIFY2(QFile::exists("test2"), "Does the new file exist after copying?");
    int second_try = QFileInfo("test2").size();

    QVERIFY2(first_try != second_try, qPrintable(QString("Second try should recreate the file: %1").arg(first_try)));

    QVERIFY(QFile::remove("test2"));

    QVERIFY2(!UpdateNode::Commander::copy("test.negative", "test2"), "File does not exist");
    QVERIFY2(!UpdateNode::Commander::copy("", ""), "Empty files are not allowed");
    QVERIFY2(!UpdateNode::Commander::copy("test.pro", ""), "Empty files are not allowed");
    QVERIFY2(!UpdateNode::Commander::copy("", "test2"), "Empty files are not allowed");

    QVERIFY2(!UpdateNode::Commander::copy("test.pro", ".."), "Copying does only work for files, not dirs");
    QVERIFY2(UpdateNode::Commander::copy("test.pro", "../test2"), "Relative copying is allowed");
    QVERIFY2(UpdateNode::Commander::copy("../unclient.rc", "test2"), "Relative copying is allowed");

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
}

void ClientTest::test_commander_run()
{
    UpdateNode::Commander commander;

    UpdateNode::Update exec_update;
    exec_update.setRequiresAdmin(false);
    exec_update.setCommand("dir");
    exec_update.setCommandLine("..");

    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    QVERIFY(commander.waitForFinished());
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));

#ifdef Q_OS_LINUX
    exec_update.setCommand("/bin/sh");
    exec_update.setCommandLine("-c\"echo this is a unit test only > test.log\"");
    QVERIFY2(commander.run(exec_update), qPrintable(exec_update.getCommand() + " " + exec_update.getCommandLine()));
    commander.waitForFinished();
    QVERIFY2(commander.getReturnCode()==0, qPrintable(QString::number(commander.getReturnCode())));
    QVERIFY2(QFile::remove("test.log"), qPrintable(commander.readStdOut()));

    exec_update.setCommand("/bin/sh");
    exec_update.setCommandLine("-c \"dir | grep cpp > test.log\"");
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
    exec_update.setCommandLine("-c \"mkdir test_dir && cp *.* test_dir\"");
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

    QNetworkReply* reply = downloader.doDownload(QUrl("http://www.updatenode.com/index.php"), update);

    QVERIFY(reply != NULL);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY(reply->error() == QNetworkReply::NoError);
}


QTEST_MAIN(ClientTest)

#include "tst_clienttest.moc"

