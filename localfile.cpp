#include <QDir>

#include "localfile.h"
#include "settings.h"
#include "config.h"

using namespace Sara;

QString LocalFile::getDownloadLocation(const QString& aFileName)
{
    return LocalFile::getDownloadPath() + QDir::separator() + QFileInfo(aFileName).fileName();
}

QString LocalFile::getDownloadPath()
{
    Sara::Settings settings;

    if(!QDir(settings.getDownloadPath() + QDir::separator() + "Sara").exists())
        QDir(settings.getDownloadPath()).mkdir("Sara");

    if(!QDir(settings.getDownloadPath() + QDir::separator() + "Sara" + QDir::separator() + Sara::Config::Instance()->getKey()).exists())
        QDir(settings.getDownloadPath() + QDir::separator() + "Sara").mkdir(Sara::Config::Instance()->getKey());

    return settings.getDownloadPath()
            + QDir::separator()
            + "Sara"
            + QDir::separator()
            + Sara::Config::Instance()->getKey();
}
