#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "logging.h"
#include <QProcessEnvironment>
#include <QSettings>
#include <QDebug>

#include "osdetection.h"
#include "wincommander.h"
#include "commander.h"
#include "settings.h"
#include "localfile.h"
#include "version.h"

using namespace UpdateNode;

Commander::Commander(QObject *parent)
    : QObject(parent)
{
    m_bCopy = false;
    m_pProcess = new QProcess(this);
    connect(m_pProcess, SIGNAL(readyReadStandardError()), this, SIGNAL(processError()));
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SIGNAL(processOutput()));
    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(updateExit(int, QProcess::ExitStatus)));
}

Commander::~Commander()
{
    m_pProcess->deleteLater();
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

void Commander::setUpdate(const Update &aUpdate)
{
    m_oUpdate = aUpdate;
}

bool Commander::run(const UpdateNode::Update& aUpdate)
{
    QString command;
    QStringList commandParameters;

    m_bCopy = false;
    m_oUpdate = aUpdate;

    command = setCommandBasedOnOS();

    QString filename = UpdateNode::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink());
    QFile file(filename);
    file.setPermissions(QFile::ExeUser | QFile::ReadUser | QFile::WriteUser);

    if(command.isEmpty())
    {
        command = resolve(m_oUpdate.getCommand());
        if(!m_oUpdate.getCommandLine().isEmpty())
            commandParameters = QStringList() << splitCommandLine(resolve(m_oUpdate.getCommandLine()));
    }
    else
    {
        commandParameters << resolve(m_oUpdate.getCommand());
        if(!m_oUpdate.getCommandLine().isEmpty())
            commandParameters << splitCommandLine(resolve(m_oUpdate.getCommandLine()));
    }

    emit progressText(tr("Installing Update '%1'").arg(m_oUpdate.getTitle()));

    UpdateNode::Logging() << "command: " << command;
    UpdateNode::Logging() << "commandlline (without quotes): " << commandParameters.join(" ");

    commandParameters.removeAll("");

    if(m_bCopy && commandParameters.size() == 2 && (!m_oUpdate.isAdminRequired() || UpdateNode::WinCommander::isProcessElevated()))
    {
        bool ret = copy(commandParameters.at(0), commandParameters.at(1));
        if(ret)
            emit updateExit(0, QProcess::NormalExit);
        else
            emit updateExit(-1, QProcess::NormalExit);
    }
    else if(m_bCopy && commandParameters.size() != 2)
    {
        UpdateNode::Logging() << "Copy command needs two parameters for source and destination file";
        emit updateExit(-2, QProcess::NormalExit);
        return false;
    }
    else
    {
        // if copy is here, it needs to be run as root. Adjust the parameter
        if(m_bCopy)
        {
            if(command.isEmpty())
            {
                command = qApp->applicationFilePath();
                commandParameters.insert(0, "-copy");
            }
            else
            {
                commandParameters.insert(0, qApp->applicationFilePath());
                commandParameters.insert(0, "-copy");
            }
        }
#ifdef Q_OS_WIN // Windows
        if(m_oUpdate.isAdminRequired() && !UpdateNode::WinCommander::isProcessElevated())
        {
            uint result = UpdateNode::WinCommander::runProcessElevated(command, commandParameters, QDir::currentPath());
            emit updateExit(result, QProcess::NormalExit);
            return true;
        }
#endif
        m_pProcess->start(command, commandParameters);

        // wait 3 minutes for process start
        if(!m_pProcess->waitForStarted(1000 * 60 * 3))
        {
            UpdateNode::Logging() << "Error: Update failed to start:" << m_pProcess->errorString();
            emit progressText(tr("Error: Update '%1' failed to start").arg(m_oUpdate.getTitle()));
            m_pProcess->kill();
            return false;
        }
    }
    return true;
}

bool Commander::waitForFinished()
{
    return m_pProcess->waitForFinished(-1);
}

int Commander::getReturnCode()
{
    return m_pProcess->exitCode();
}

bool Commander::copy(const QString& aFrom, const QString& aTo)
{
    if(aFrom.isEmpty() || aTo.isEmpty())
        return false;

    QFile file(aFrom);
    QFile destFile(aTo);

    if(!file.exists())
        return false;

    if(QFileInfo(destFile).isFile() && destFile.exists())
        if(!destFile.remove())
            return false;

    if(file.copy(aTo))
        return true;
    else
        return false;
}

QString Commander::readStdErr() const
{
    return m_pProcess->readAllStandardError();
}

QString Commander::readStdOut() const
{
    return m_pProcess->readAllStandardOutput();
}

/// command = "ld.exe [HOME] [INI@/home/user/.config/UpdateNode/Client.conf:uuid] [SHELL] [@[HOME]/.config/UpdateNode/Client.conf:uuid]";
QString Commander::resolve(const QString& aString)
{
    UpdateNode::Settings settings;
    QString theString = aString;

    // replace internals
    theString = theString.replace("[UN_UP_CODE]", m_oUpdate.getCode());
    theString = theString.replace("[UN_UP_LINK]", m_oUpdate.getDownloadLink());
    theString = theString.replace("[UN_UP_SIZE]", m_oUpdate.getFileSize());
    theString = theString.replace("[UN_UP_TARGETVERSION]", m_oUpdate.getTargetVersion().getVersion());
    theString = theString.replace("[UN_UP_TARGETCODE]", m_oUpdate.getTargetVersion().getCode());
    theString = theString.replace("[UN_UP_TYPE]", QString::number(m_oUpdate.getType()));
    theString = theString.replace("[UN_FILE]", UpdateNode::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink()));
    theString = theString.replace("[UN_FILENAME]", QFileInfo(UpdateNode::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink())).fileName());
    theString = theString.replace("[UN_FILEEXT]", QFileInfo(UpdateNode::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink())).completeSuffix());

    if(theString.indexOf("[UN_COPY_COMMAND]")>-1)
        m_bCopy = true;

    return resolveGeneral(theString);
}

QString Commander::resolveGeneral(const QString& aString)
{
    UpdateNode::Settings settings;
    QString theString = aString;

    // replace internals
    theString = theString.replace("[UN_SEP]", QDir::separator());
    theString = theString.replace("[UN_DOWNLOAD_PATH]", UpdateNode::LocalFile::getDownloadPath());
    theString = theString.replace("[UN_CLIENT_PATH]", settings.getCurrentClientDir());
    theString = theString.replace("[UN_VERSION]", UPDATENODE_CLIENT_VERSION);
    theString = theString.replace("[UN_LANG]", UpdateNode::Config::Instance()->getLanguage());
    theString = theString.replace("[UN_OS]", UpdateNode::OSDetection::getOS());
    theString = theString.replace("[UN_ARCH]", UpdateNode::OSDetection::getArch());

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

    // remove letfovers
    theString = theString.replace(QRegExp("\\[[^]]*\\]"), "");

    return theString;
}

QStringList Commander::splitCommandLine(const QString &aString)
{
    QStringList list;

    int pos = aString.indexOf("\"");
    if(pos>-1)
    {
        list << aString.left(pos).split(" ", QString::SkipEmptyParts);
        int end = aString.indexOf("\"", pos+1);
        if(end>-1)
            list << aString.mid(pos+1, end-pos-1);
        else
            return aString.split(" ", QString::SkipEmptyParts);

        return list << splitCommandLine(aString.mid(end+1));
    }
    else
        return aString.split(" ", QString::SkipEmptyParts);
}


