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
