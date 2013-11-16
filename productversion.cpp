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

/*!
\class UpdateNode::ProductVersion
\brief The UpdateNode::ProductVersion class holds informations about the product version, returned by UpdateNode.com
\n\n
Using this class, you can access all product version data coming from the server
*/

/*!
Constructs an empty ProductVersion object.
*/
ProductVersion::ProductVersion()
{
}

/*!
Sets the product version name
\sa ProductVersion::getName
*/
void ProductVersion::setName(const QString& aName)
{
    m_strName = aName;
}

/*!
Returns the product version name. The returned string is empty when no name was set
\sa ProductVersion::setName
*/
QString ProductVersion::getName() const
{
    return m_strName;
}

/*!
Sets the product version code
\sa ProductVersion::getCode
*/
void ProductVersion::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

/*!
Returns the product version code. The returned string is empty when no code was set
\sa ProductVersion::setCode
*/
QString ProductVersion::getCode() const
{
    return m_strCode;
}

/*!
Sets the product version
\note Your product version should have following default version schema: aaaa.bbbb.cccc.dddd
\n For example
~~~~~
1.23.0.23422
~~~~~
\sa ProductVersion::getVersion
*/
void ProductVersion::setVersion(const QString& aVersion)
{
    m_strVersion = aVersion;
}

/*!
Returns the product version. The returned string is empty when no version was set
\sa ProductVersion::setVersion
*/
QString ProductVersion::getVersion() const
{
    return m_strVersion;
}
