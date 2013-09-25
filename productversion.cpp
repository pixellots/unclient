#include "productversion.h"

using namespace UpdateNode;

ProductVersion::ProductVersion()
{

}

void ProductVersion::setName(const QString& aName)
{
    m_strName = aName;
}

QString ProductVersion::getName() const
{
    return m_strName;
}

void ProductVersion::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

QString ProductVersion::getCode() const
{
    return m_strCode;
}

void ProductVersion::setVersion(const QString& aVersion)
{
    m_strVersion = aVersion;
}

QString ProductVersion::getVersion() const
{
    return m_strVersion;
}
