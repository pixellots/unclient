/****************************************************************************
**
** Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
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

/*!
\class UpdateNode::Product
\brief The UpdateNode::Product class holds informations about the product, returned by UpdateNode.com
\n\n
Using this class, you can access all product data coming from the server
*/

/*!
Constructs an empty Product object.
*/
Product::Product()
{
}

/*!
Sets the product name
\sa Product::getName
*/
void Product::setName(const QString& aName)
{
    m_strName = aName;
}

/*!
Returns the product name. The returned string is empty when no name was set
\sa Product::setName
*/
QString Product::getName() const
{
    return m_strName;
}

/*!
Sets the product code
\sa Product::getCode
*/
void Product::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

/*!
Returns the product code. The returned string is empty when no code was set
\sa Product::setCode
*/
QString Product::getCode() const
{
    return m_strCode;
}

/*!
Sets the product icon URL
\sa Product::getIconUrl
*/
void Product::setIconUrl(const QString& aUrl)
{
    m_strIconUrl = aUrl;
}

/*!
Returns the product icon URL. The returned string is empty when no URL was set
\sa Product::setIconUrl
*/
QString Product::getIconUrl() const
{
    return m_strIconUrl;
}

/*!
Returns the product icon URL within its local download location.
The returned string will contain the download path, even no icon URL is set before.
\n
This method calls UpdateNode::LocalFile::getDownloadLocation to detect the download path
\sa UpdateNode::LocalFile::getDownloadLocation
\sa Product::setIconUrl
\sa Product::getIconUrl
*/
QString Product::getLocalIcon() const
{
    return UpdateNode::LocalFile::getDownloadLocation(m_strIconUrl);
}
