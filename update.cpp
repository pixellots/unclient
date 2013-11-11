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

#include "update.h"
#include "commander.h"

#include <QUrl>

using namespace UpdateNode;

Update::Update()
{
}

void Update::setTitle(const QString& aTitle)
{
    m_strTitle = aTitle;
}

QString Update::getTitle() const
{
    return m_strTitle;
}

void Update::setDescription(const QString& aDescription)
{
    m_strDescription = aDescription;
}

QString Update::getDescription() const
{
    return m_strDescription;
}

void Update::setDownloadLink(const QString& aDownloadLink)
{
    m_strDownloadLink = aDownloadLink;
}

QString Update::getDownloadLink() const
{
    return QUrl::fromUserInput(UpdateNode::Commander::resolveGeneral(m_strDownloadLink)).toEncoded();
}

void Update::setCommand(const QString& aCommand)
{
    m_strCommand = aCommand;
}

QString Update::getCommand() const
{
    return m_strCommand;
}

void Update::setCommandLine(const QString& aCommandLine)
{
    m_strCommandLine = aCommandLine;
}

QString Update::getCommandLine() const
{
    return m_strCommandLine;
}

void Update::setRequiresAdmin(bool aAdminRequired)
{
    m_bAdminRequired = aAdminRequired;
}

bool Update::isAdminRequired() const
{
    return m_bAdminRequired;
}

void Update::setFileSize(const QString& aFileSize)
{
    m_strFileSize = aFileSize;
}

QString Update::getFileSize() const
{
    return m_strFileSize;
}

void Update::setType(int aType)
{
    m_iType = aType;
}

int Update::getType() const
{
    return m_iType;
}

Update::Type Update::getTypeEnum()
{
    return (Update::Type)m_iType;
}

void Update::setTargetVersion(const ProductVersion& aTarget)
{
    m_oTarget = aTarget;
}

ProductVersion Update::getTargetVersion() const
{
    return m_oTarget;
}

void Update::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

QString Update::getCode() const
{
    return m_strCode;
}

