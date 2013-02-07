#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include "commander.h"

using namespace Sara;

Commander::Commander(QObject *parent)
    : QObject(parent)
{
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

    m_oUpdate = aUpdate;

    command = setCommandBasedOnOS();

    QString filename = QDir::tempPath() + QDir::separator() + "Sara" + QDir::separator() + QFileInfo(m_oUpdate.getDownloadLink()).fileName();
    QFile file(filename);
    file.setPermissions(QFile::ExeUser | QFile::ReadUser | QFile::WriteUser);
        if(command.isEmpty())
    {
        command = m_oUpdate.getCommand();
        if(!m_oUpdate.getCommandLine().isEmpty())
            commandParameters = QStringList() << m_oUpdate.getCommandLine().split(" ");
    }
    else
    {
        commandParameters << m_oUpdate.getCommand();
        if(!m_oUpdate.getCommandLine().isEmpty())
            commandParameters << m_oUpdate.getCommandLine().split(" ");
    }

    emit progressText(tr("Installing Update '%1'").arg(m_oUpdate.getTitle()));

    qDebug() << "command: " << command;
    qDebug() << "commandlline: " << commandParameters.join(" ");

    m_pProcess->start(command, commandParameters);

    // wait 5 minutes for process start
    if(!m_pProcess->waitForStarted(1000 * 60 * 5))
    {
        emit progressText(tr("Error: Update '%1' failed to start").arg(m_oUpdate.getTitle()));
        m_pProcess->kill();
        return false;
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

QString Commander::resolveCommand(const QString& aCommand) const
{
    return QString();
}

QStringList Commander::resolveCommandLineParameters(const QStringList& aCommandLineParameter) const
{
    return QStringList();
}

QString Commander::resolve(const QString& aString) const
{
    return QString();
}

