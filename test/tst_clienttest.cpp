#include <QtCore/QString>
#include <QtTest/QtTest>

#include "../commander.h"
#include "../update.h"
#include "../productversion.h"
#include "../version.h"

class ClientTest : public QObject
{
    Q_OBJECT
    
public:
    ClientTest();
    
private Q_SLOTS:
    void test_resolve();
    void test_version();

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
}

void ClientTest::test_resolve()
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
}

void ClientTest::test_version()
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

QTEST_APPLESS_MAIN(ClientTest)

#include "tst_clienttest.moc"

