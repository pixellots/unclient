#include <QtCore/QString>
#include <QtTest/QtTest>

#include "../commander.h"
#include "../update.h"
#include "../productversion.h"

class CommanderTest : public QObject
{
    Q_OBJECT
    
public:
    CommanderTest();
    
private Q_SLOTS:
    void test_resolve();

private:
    UpdateNode::Update update;
};

CommanderTest::CommanderTest()
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

void CommanderTest::test_resolve()
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

QTEST_APPLESS_MAIN(CommanderTest)

#include "tst_commandertest.moc"
