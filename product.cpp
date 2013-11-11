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
#include "product.h"
#include "localfile.h"

using namespace UpdateNode;

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
    return UpdateNode::LocalFile::getDownloadLocation(m_strIconUrl);
}
