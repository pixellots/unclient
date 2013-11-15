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

/*!
\class UpdateNode::Commander
\brief The UpdateNode::Commander class is used to start external programs,
defined in UpdateNode.com
\n\n
This class is mainly used to execute programs with and without root privilages.
It provides methods to resolve variables embedded in the command, or commandline
parameters.
\n
Variables need to be embed in brakets. For exampple: [UN_FILE] or [UN_FILENAME][UN_FILE_EXT]
\n
unclient supports 4 types of variables:
\n
- Native settings
- INI settings
- Internal variables
- Environment variables

\n \n
These are the possible variables which can be resolved within a command or commandline parameter definition:
\n
\pre Native Settings
-------------------------
Native settings depends on the selected platform. On Windows, this refer to the registry, on Linux that an INI format and on Mac OS it is plist format by default.
\n
To get values from your native settings, use following syntax:
\n
~~~~~~~~~~~~~~~~~~~~~
[@<PATH>:<KEY>]
~~~~~~~~~~~~~~~~~~~~~
\n
Examples:
\n
~~~~~~~~~~~~~~~~~~~~~
[@HKEY_CURRENT_USER\\MySoft\\Star Runner\\Galaxy:General]
~~~~~~~~~~~~~~~~~~~~~
or
~~~~~~~~~~~~~~~~~~~~~
[@[HOME]/.config/UpdateNode/Client.conf:uuid]
~~~~~~~~~~~~~~~~~~~~~
\n
INI Settings
-------------------------
You can read ini files from on any system using the following syntax:
\n
~~~~~~~~~~~~~~~~~~~~~
[INI@<PATH>:<KEY>]
~~~~~~~~~~~~~~~~~~~~~
\n
Examples:
\n
~~~~~~~~~~~~~~~~~~~~~
[INI@[HOME][UN_SEP]settings.ini:Language]
~~~~~~~~~~~~~~~~~~~~~
\n
Internal Variables (Connected to your update definition of www.updatenode.com)
-------------------------
Variable            | Description                         | Example
-------------       | -------------                       | -------------
UN_FILE             | The downloaded file                 | c:\\temp\\setup.exe
UN_FILENAME         | File name of the downloaded file    | setup.exe
UN_FILEEXT          | The extension of the downloaded file| exe
UN_UP_SIZE_REAL     | Real size of the downloaded file\n in Bytes| 25444000
UN_UP_CODE          | The update code which is internally\n used to identify your update | {c7a20520-4dcf-11e3-94e8-0002a5d5c51b}
UN_UP_LINK          | The download link\n UpdateNode:::Commander::resolveGeneral resolved | http://updatenode.com/example/custom_value/setup.exe
UN_UP_SIZE          | Size of the downloaded file as specifiec in UpdateNode.com | 25 MB
UN_UP_TARGETVERSION | The update's target version         | 2.0
UN_UP_TARGETCODE    | The update target's version code    | my_new_version_code_1234
UN_UP_TYPE          | The update type (Installer (1), or \n client based update (2) | 1
\n\n
Internal Variables (Not related to www.updatenode.com settings)
-------------------------
Variable            | Description                         | Example
-------------       | -------------                       | -------------
UN_SEP              | Native separator                    | \\ or /
UN_DOWNLOAD_PATH    | The download path where all file are\n downloaded thru | /tmp/UpdateNode/62525142523515263
UN_CLIENT_PATH      | The directory where the unclient is \n launched from | C:\\Program Files\\My Product\\Update
UN_VERSION          | The current unclient version        | 1.0
UN_LANG             | Language set in unclient, or system\n language if no language was specified | de-DE
UN_OS               | Operating system's name and version | Windows 6.2
UN_ARCH             | Operating system's architecture     | x86

*/

/*!
Constructs a Commander object with the given parent.
*/
Commander::Commander(QObject *parent)
    : QObject(parent)
{
    m_bCopy = false;
    m_pProcess = new QProcess(this);
    connect(m_pProcess, SIGNAL(readyReadStandardError()), this, SIGNAL(processError()));
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SIGNAL(processOutput()));
    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(updateExit(int, QProcess::ExitStatus)));
}

/*!
Destructs the Commander object.
*/
Commander::~Commander()
{
    m_pProcess->deleteLater();
}

/*!
Predefines the command which needs to be executed based on internal decisions.
*/
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

/*!
Sets the current Update object required to resolve internal variables
*/
void Commander::setUpdate(const Update &aUpdate)
{
    m_oUpdate = aUpdate;
}

/*!
Resolves and runs the update as specified in Update.
\n Retruns true on success, returns false when the process was not able to start.
\note This function emits the signal Commander::updateExit in each situation
*/
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
#ifdef Q_OS_UNIX
                commandParameters = splitCommandLine("\"" + resolve(qApp->applicationFilePath() + " -copy " + m_oUpdate.getCommandLine()) + "\"");
#else
                commandParameters.insert(0, "-copy");
                commandParameters.insert(0, qApp->applicationFilePath());
#endif
            }
        }

        UpdateNode::Logging() << "command: " << command;
        UpdateNode::Logging() << "commandline (without quotes): " << commandParameters.join(" ");

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

/*!
Waits forever until the current running process is not finished
\n This function always returns true, as waitForFinished is called without timeout
*/
bool Commander::waitForFinished()
{
    return m_pProcess->waitForFinished(-1);
}

/*!
Returns the exit code from the last run process
*/
int Commander::getReturnCode()
{
    return m_pProcess->exitCode();
}

/*!
Copies a file from \a aFrom to \a aTo. Both parameters needs to be files, not directories.
\n
Returns true if the copy command was successfully, or false in case the destination \a aTo
file cannot be deleted or the copy command failes due to another reasons
*/
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

/*!
Returns data which has been written to stderr
*/
QString Commander::readStdErr() const
{
    return m_pProcess->readAllStandardError();
}

/*!
Returns data which has been written to stdout
*/
QString Commander::readStdOut() const
{
    return m_pProcess->readAllStandardOutput();
}

/*!
Resolved all variables passed as \a aString and returns the resolved string
\note [UN_COPY_COMMAND] is a special variable which enforces to execute the UpdateNode::Commander::copy method
*/
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
    theString = theString.replace("[UN_FILE_SIZE]",QString::number( QFileInfo(UpdateNode::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink())).size()));
    theString = theString.replace("[UN_FILENAME]", QFileInfo(UpdateNode::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink())).fileName());
    theString = theString.replace("[UN_FILEEXT]", QFileInfo(UpdateNode::LocalFile::getDownloadLocation(m_oUpdate.getDownloadLink())).completeSuffix());

    if(theString.indexOf("[UN_COPY_COMMAND]")>-1)
        m_bCopy = true;

    return resolveGeneral(theString);
}

/*!
Resolves all independent variables from the given string \a aString and
returns the resolved string.
*/
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

/*!
Command line parameters gets splitted in by quotes
\n
~~~~~~~~~~~~~~~~~~~~~
QStringList list = UpdateNode::Commander::splitCommandLine("-i main.png -v 1.2.0 \\"This should be in quotes\\" hello");
~~~~~~~~~~~~~~~~~~~~~
will result in a QStringList with following items:

- -i
- main.png
- -v
- 1.2.0
- This should be in quotes
- hello
*/
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


