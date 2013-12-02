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

/*!
\class UpdateNode::Message
\brief The UpdateNode::Message class holds informations about the message, returned by UpdateNode.com
\n\n
Using this class, you can access all message data coming from the server
*/

/*!
Constructs an empty Message object.
*/
Message::Message()
{
}

/*!
Sets the message title
\sa Message::getTitle
*/
void Message::setTitle(const QString& aTitle)
{
    m_strTitle = aTitle;
}

/*!
Returns the message title. The returned string is empty when no title was set
\sa Message::setTitle
*/
QString Message::getTitle() const
{
    return m_strTitle;
}

/*!
Sets the message text
\sa Message::getMessage
*/
void Message::setMessage(const QString& aMessage)
{
    m_strMessage = aMessage;
}

/*!
Returns the message text. The returned string is empty when no text was set
\sa Message::setMessage
*/
QString Message::getMessage() const
{
    return m_strMessage;
}

/*!
Sets the link to the message file
\sa Message::getLink
*/
void Message::setLink(const QString& aLink)
{
    m_strLink = aLink;
}

/*!
Returns the message link. The returned string gets resolved thru
UpdateNode::Commander::resolveGeneral. When the string is not already a valid URL,
a best guess is performed, making various web related assumptions
\sa Message::setLink
*/
QString Message::getLink() const
{
    return UpdateNode::Commander::resolveGeneral(QUrl::fromUserInput(m_strLink).toEncoded());
}

/*!
Sets the message code
\sa Message::getCode
*/
void Message::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

/*!
Returns the message code. The returned string is empty when no code was set
\sa Message::setCode
*/
QString Message::getCode() const
{
    return m_strCode;
}

void Message::setOpenExternal(bool aEnable)
{
    m_bOpenExternal = aEnable;
}

bool Message::isOpenExternal() const
{
    return m_bOpenExternal;
}


