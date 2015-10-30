/****************************************************************************
**
** Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
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

#include <QDir>

#include "localfile.h"
#include "settings.h"
#include "config.h"

using namespace UpdateNode;

/*!
\class UpdateNode::LocalFile
\brief Class container for handeling file locations
*/

/*!
Constructs a file location for a gieven \a aFileName based on its name and the result of
\n LocalFile::getDownloadPath
*/
QString LocalFile::getDownloadLocation(const QString& aFileName)
{
    QFileInfo info(aFileName);
    QString fileName = info.fileName();

    fileName = fileName.split('?').at(0);

    return QDir::toNativeSeparators(LocalFile::getDownloadPath() + QDir::separator() + fileName);
}

/*!
Returns the download path as specified using Settings::setDownloadPath
*/
QString LocalFile::getDownloadPath()
{
    UpdateNode::Settings settings;

    if(!QDir(settings.getDownloadPath() + QDir::separator() + "UpdateNode").exists())
        QDir(settings.getDownloadPath()).mkdir("UpdateNode");

    if(!QDir(settings.getDownloadPath() + QDir::separator() + "UpdateNode" + QDir::separator() + UpdateNode::Config::Instance()->getKeyHashed()).exists())
        QDir(settings.getDownloadPath() + QDir::separator() + "UpdateNode").mkdir(UpdateNode::Config::Instance()->getKeyHashed());

    return QDir::toNativeSeparators(settings.getDownloadPath()
            + QDir::separator()
            + "UpdateNode"
            + QDir::separator()
            + UpdateNode::Config::Instance()->getKeyHashed());
}


/*!
Returns the cache path which is the download path with a subfolder called "cache"
*/
QString LocalFile::getCachePath()
{
    return QDir::toNativeSeparators(getDownloadPath() + QDir::separator() + "cache");
}
