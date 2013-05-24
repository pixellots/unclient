#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QProcessEnvironment>
#include <QSettings>
#include "commander.h"
#include "settings.h"
#include "localfile.h"
#include "version.h"

using namespace Sara;

Commander::Commander(QObject *parent)
    : QObject(parent)
{
    m_bCopy = false;
    m_pProcess = new QProcess(this);
    connect(m_pProcess, SIGNAL(readyReadStandardError()), this, SIGNAL(processError()));
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SIGNAL(processOutput()));
    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(updateExit(int, QProcess::ExitStatus)));
}

QString Commander::setCommandBasedOnOS() const
{
    QString command;
#ifdef Q_OS_LINUX // Linux
    if(m_oUpdate.isAdminRequired())
    {
        QString desktop = getenv("DESKTOP_SESSION");
        if(desktop.indexOf("kubuntu") != -1 || desktop.indexOf("kde") != -1)
            command = "/usr/bin/kdesudo";
        else
            command = "/usr/bin/gksudo";
    }
#else
#ifdef Q_OS_WIN // Windows
// Do nothing on Windows
#else
#ifdef Q_OS_MAC // Mac
// Do nothing on Mac
#endif
#endif
#endif
    return command;
}

bool Commander::run(const Sara::Update& aUpdate)
{
    QString command;
    QStringList commandParameters;

    m_bCopy = false;
    m_oUpdate = aUpdate;

    command = setCommandBasedOnOS();

    QString filename = Sara::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink());
    QFile file(filename);
    file.setPermissions(QFile::ExeUser | QFile::ReadUser | QFile::WriteUser);

    if(command.isEmpty())
    {
        command = resolve(m_oUpdate.getCommand());
        if(!m_oUpdate.getCommandLine().isEmpty())
            commandParameters = QStringList() << resolve(m_oUpdate.getCommandLine()).split(" ");
    }
    else
    {
        commandParameters << resolve(m_oUpdate.getCommand());
        if(!m_oUpdate.getCommandLine().isEmpty())
            commandParameters << resolve(m_oUpdate.getCommandLine()).split(" ");
    }

    emit progressText(tr("Installing Update '%1'").arg(m_oUpdate.getTitle()));

    qDebug() << "command: " << command;
    qDebug() << "commandlline: " << commandParameters.join(" ");

    commandParameters.removeAll("");

    if(m_bCopy && commandParameters.size() > 1)
    {
        QFile file(commandParameters.at(0));
        QFile destFile(commandParameters.at(1));

        if(!file.exists())
        {
            emit updateExit(-1, QProcess::NormalExit);
            return false;
        }

        if(QFileInfo(destFile).isFile() && destFile.exists())
            if(!destFile.remove())
            {
                emit updateExit(-2, QProcess::NormalExit);
                return false;
            }

        if(file.copy(commandParameters.at(1)))
        {
            emit updateExit(0, QProcess::NormalExit);
            return true;
        }
        else
        {
            qDebug() << file.errorString();
            emit updateExit(file.error(), QProcess::NormalExit);
            return false;
        }
    }
    else
    {
        m_pProcess->start(command, commandParameters);

        // wait 5 minutes for process start
        if(!m_pProcess->waitForStarted(1000 * 60 * 5))
        {
            emit progressText(tr("Error: Update '%1' failed to start").arg(m_oUpdate.getTitle()));
            m_pProcess->kill();
            return false;
        }
    }
    return true;
}

QString Commander::readStdErr() const
{
    return m_pProcess->readAllStandardError();
}

QString Commander::readStdOut() const
{
    return m_pProcess->readAllStandardOutput();
}

/// command = "ld.exe [HOME] [INI@/home/user/.config/Sara/Client.conf:uuid] [SHELL] [@[HOME]/.config/Sara/Client.conf:uuid]";
QString Commander::resolve(const QString& aString)
{
    QString theString = aString;

    // replace internals
    theString = theString.replace("[SARA_UP_CODE]", m_oUpdate.getCode());
    theString = theString.replace("[SARA_UP_LINK]", m_oUpdate.getDownloadLink());
    theString = theString.replace("[SARA_UP_SIZE]", m_oUpdate.getFileSize());
    theString = theString.replace("[SARA_UP_TARGETVERSION]", m_oUpdate.getTargetVersion().getVersion());
    theString = theString.replace("[SARA_UP_TARGETCODE]", m_oUpdate.getTargetVersion().getCode());
    theString = theString.replace("[SARA_UP_TYPE]", QString::number(m_oUpdate.getType()));
    theString = theString.replace("[SARA_FILE]", Sara::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink()));
    theString = theString.replace("[SARA_FILENAME]", QFileInfo(Sara::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink())).fileName());
    theString = theString.replace("[SARA_FILEEXT]", QFileInfo(Sara::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink())).completeSuffix());
    theString = theString.replace("[SARA_SEP]", QDir::separator());
    theString = theString.replace("[SARA_PATH]", Sara::LocalFile::getDownloadPath());
    theString = theString.replace("[SARA_VERSION]", SARA_CLIENT_VERSION);

    // replace environment vars
    QStringList env = QProcessEnvironment::systemEnvironment().toStringList();

    foreach(QString string, env)
    {
        QString key = string.split('=').at(0);
        QString value = string.split('=').at(1);

        theString = theString.replace("["+key+"]", value);
    }

    // replace native settings
    int pos = theString.indexOf(QString("[@"));
    while(pos!=-1)
    {
        int rpos = theString.indexOf(']', pos);
        QString subString = theString.mid(pos + 2, rpos - pos - 2);
        QString path = subString.split(':').at(0);
        QString key = subString.split(':').at(1);

        QSettings settings(path, QSettings::NativeFormat);

        theString = theString.left(pos) + settings.value(key, "").toString() + theString.right(theString.length()-rpos-1);

        pos = theString.indexOf(QString("[@"), pos+1);
    }

    // replace ini settings
    pos = theString.indexOf(QString("[INI@"));
    while(pos!=-1)
    {
        int rpos = theString.indexOf(']', pos);
        QString subString = theString.mid(pos + 5, rpos - pos - 5);
        QString path = subString.split(':').at(0);
        QString key = subString.split(':').at(1);

        QSettings settings(path, QSettings::IniFormat);

        theString = theString.left(pos) + settings.value(key, "").toString() + theString.right(theString.length()-rpos-1);

        pos = theString.indexOf(QString("[@"), pos+1);
    }

    if(theString.indexOf("[SARA_COPY_COMMAND]")>-1)
        m_bCopy = true;

    // remove letfovers
    theString = theString.replace(QRegExp("\\[[^]]*\\]"), "");

    return theString;
}
