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

#include <QDir>

#include "localfile.h"
#include "settings.h"
#include "config.h"

using namespace UpdateNode;

QString LocalFile::getDownloadLocation(const QString& aFileName)
{
    return QDir::toNativeSeparators(LocalFile::getDownloadPath() + QDir::separator() + QFileInfo(aFileName).fileName());
}

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


QString LocalFile::getCachePath()
{
    return QDir::toNativeSeparators(getDownloadPath() + QDir::separator() + "cache");
}
