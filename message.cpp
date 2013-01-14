#include "message.h"

using namespace Sara;

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
    return m_strLink;
}
