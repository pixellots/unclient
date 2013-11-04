#include <QDir>

#include "localfile.h"
#include "settings.h"
#include "config.h"

using namespace UpdateNode;

QString LocalFile::getDownloadLocation(const QString& aFileName)
{
    return LocalFile::getDownloadPath() + QDir::separator() + QFileInfo(aFileName).fileName();
}

QString LocalFile::getDownloadPath()
{
    UpdateNode::Settings settings;

    if(!QDir(settings.getDownloadPath() + QDir::separator() + "UpdateNode").exists())
        QDir(settings.getDownloadPath()).mkdir("UpdateNode");

    if(!QDir(settings.getDownloadPath() + QDir::separator() + "UpdateNode" + QDir::separator() + UpdateNode::Config::Instance()->getKey()).exists())
        QDir(settings.getDownloadPath() + QDir::separator() + "UpdateNode").mkdir(UpdateNode::Config::Instance()->getKey());

    return settings.getDownloadPath()
            + QDir::separator()
            + "UpdateNode"
            + QDir::separator()
            + UpdateNode::Config::Instance()->getKey();
}


QString LocalFile::getCachePath()
{
    return getDownloadPath() + QDir::separator() + "cache";
}
