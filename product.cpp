#include <QDir>
#include "product.h"

using namespace Sara;

Product::Product()
{
}

void Product::setName(const QString& aName)
{
    m_strName = aName;
}

QString Product::getName() const
{
    return m_strName;
}

void Product::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

QString Product::getCode() const
{
    return m_strCode;
}

void Product::setIconUrl(const QString& aUrl)
{
    m_strIconUrl = aUrl;
}

QString Product::getIconUrl() const
{
    return m_strIconUrl;
}

QString Product::getLocalIcon() const
{
    if(!QDir(QDir::tempPath() + QDir::separator() + "Sara").exists())
        QDir(QDir::tempPath()).mkdir("Sara");
    return QDir::tempPath() + QDir::separator() + "Sara" + QDir::separator() + QFileInfo(m_strIconUrl).fileName();
}
