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

#include "binarysettings.h"
#include <QTemporaryFile>
#include <QStringList>

using namespace UpdateNode;

/*!
\class UpdateNode::BinarySettings
\brief Helper class for a custom binary QSettings reader/writer
*/

/*!
Reads settings from a binary file
\sa BinarySettings::writeBinFile
*/
bool BinarySettings::readBinFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QTemporaryFile file;

    file.open();

    QByteArray array = qUncompress(device.readAll());
    device.close();

    file.write(array, array.size());
    file.flush();

    QSettings *settings = new QSettings(file.fileName(), QSettings::IniFormat);
    QStringList keys = settings->allKeys();

    foreach(QString key, keys)
        map.insert(key, settings->value(key));

    delete settings;
    return true;
}

/*!
Writes settings to a binary file
\sa BinarySettings::readBinFile
*/
bool BinarySettings::writeBinFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    QTemporaryFile file;

    file.open();

    QSettings *settings = new QSettings(file.fileName(), QSettings::IniFormat);

    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
         i.next();
         settings->setValue(i.key(), i.value());
    }
    delete settings;

    QByteArray array = file.readAll();
    file.close();

    array = qCompress(array);

    device.write(array, array.size());
    device.close();

    return true;
}

