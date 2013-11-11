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

#include "message.h"
#include "commander.h"

#include <QUrl>

using namespace UpdateNode;

Message::Message()
{
}

void Message::setTitle(const QString& aTitle)
{
    m_strTitle = aTitle;
}

QString Message::getTitle() const
{
    return m_strTitle;
}

void Message::setMessage(const QString& aMessage)
{
    m_strMessage = aMessage;
}

QString Message::getMessage() const
{
    return m_strMessage;
}

void Message::setLink(const QString& aLink)
{
    m_strLink = aLink;
}

QString Message::getLink() const
{
    return UpdateNode::Commander::resolveGeneral(QUrl::fromUserInput(m_strLink).toEncoded());
}

void Message::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

QString Message::getCode() const
{
    return m_strCode;
}

