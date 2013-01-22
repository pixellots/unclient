#include "update.h"

using namespace Sara;

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
    return m_strDownloadLink;
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

bool Update::isAdminRequired()
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

int Update::getType()
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
