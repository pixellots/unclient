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

#include "update.h"
#include "commander.h"

#include <QUrl>

using namespace UpdateNode;

/*!
\class UpdateNode::Update
\brief The UpdateNode::Update class holds informations about the update, returned by UpdateNode.com
\n\n
Using this class, you can access all update data coming from the server
*/

/*!
Constructs an empty Update object.
*/
Update::Update()
{
    m_bAdminRequired = false;
    m_bMandatory = false;
    m_iType = 1;
}

/*!
Sets the update title
\sa Update::getTitle
*/
void Update::setTitle(const QString& aTitle)
{
    m_strTitle = aTitle;
}

/*!
Returns the update title. The returned string is empty when no title was set
\sa Update::setTitle
*/
QString Update::getTitle() const
{
    return m_strTitle;
}

/*!
Sets the update description
\sa Update::getDescription
*/
void Update::setDescription(const QString& aDescription)
{
    m_strDescription = aDescription;
}

/*!
Returns the update description. The returned string is empty when no description was set
\sa Update::setDescription
*/
QString Update::getDescription() const
{
    return m_strDescription;
}

/*!
Sets the update download link
\sa Update::getDownloadLink
*/
void Update::setDownloadLink(const QString& aDownloadLink)
{
    m_strDownloadLink = aDownloadLink;
}

/*!
Returns the update link. The returned string gets resolved thru
UpdateNode::Commander::resolveGeneral. When the string is not already a valid URL,
a best guess is performed, making various web related assumptions
\sa Update::setDownloadLink
*/
QString Update::getDownloadLink() const
{
    return QUrl::fromUserInput(UpdateNode::Commander::resolveGeneral(m_strDownloadLink)).toEncoded();
}

/*!
Sets the update command
\sa Update::getCommand
*/
void Update::setCommand(const QString& aCommand)
{
    m_strCommand = aCommand;
}

/*!
Returns the update command. The returned string is empty when no command was set
\sa Update::setCommand
*/
QString Update::getCommand() const
{
    return m_strCommand;
}

/*!
Sets the update command line parameters
\sa Update::getCommandLine
*/
void Update::setCommandLine(const QString& aCommandLine)
{
    m_strCommandLine = aCommandLine;
}

/*!
Returns the update command line parameters as a string. The returned string is empty when no command line parameter was set
\sa Update::setCommandLine
*/
QString Update::getCommandLine() const
{
    return m_strCommandLine;
}

/*!
Defines if the update should run with administrative privileges, or without
\n Default is without admin rights
\sa Update::isAdminRequired
*/
void Update::setRequiresAdmin(bool aAdminRequired)
{
    m_bAdminRequired = aAdminRequired;
}

/*!
This method returns true if the update requires administrative rights. Default value is false.
\sa Update::setRequiresAdmin
*/
bool Update::isAdminRequired() const
{
    return m_bAdminRequired;
}

/*!
Sets the update file size of the file as spefified in UpdateNode.com
\note This value is a human readable string which can be changed within your UpdateNode account
\sa Update::getFileSize
*/
void Update::setFileSize(const QString& aFileSize)
{
    m_strFileSize = aFileSize;
}

/*!
Returns the update file size as a string. The returned string is empty when size was not specified
\sa Update::setFileSize
*/
QString Update::getFileSize() const
{
    return m_strFileSize;
}

/*!
Sets the update type
\note Currently there are only two types of update: INSTALLER_SETS_VERSION = 1 and CLIENT_SETS_VERSION = 2
\sa Update::getType
*/
void Update::setType(int aType)
{
    m_iType = aType;
}

/*!
Returns the update type as integer
\sa Update::setType
*/
int Update::getType() const
{
    return m_iType;
}

/*!
Returns the update type as Enum Update::Type
\sa Update::setType
\sa Update::getType
*/
Update::Type Update::getTypeEnum()
{
    return (Update::Type)m_iType;
}

/*!
Sets the update target version.
\sa Update::getTargetVersion
\sa Update::ProductVersion
*/
void Update::setTargetVersion(const ProductVersion& aTarget)
{
    m_oTarget = aTarget;
}

/*!
Returns the update target version.
\sa Update::setTargetVersion
\sa Update::ProductVersion
*/
ProductVersion Update::getTargetVersion() const
{
    return m_oTarget;
}

/*!
Sets the update code
\sa Update::getCode
*/
void Update::setCode(const QString& aCode)
{
    m_strCode = aCode;
}

/*!
Returns the update code. The returned string is empty when no code was set
\sa Update::setCode
*/
QString Update::getCode() const
{
    return m_strCode;
}

/*!
Returns whether this update can be ignored or not
\sa Update::setMandatory
*/
bool Update::isMandatory() const
{
    return m_bMandatory;
}

/*!
Sets the mandatory flag for a update. By default an update is not mandatory
\sa Update::isMandatory
*/
void Update::setMandatory(bool aMandatoryUpdate)
{
    m_bMandatory = aMandatoryUpdate;
}

/*!
Returns the checksum of the download file
\sa Update::getChecksum
*/
void Update::setChecksum(const QString& aChecksum)
{
    m_strChecksum = aChecksum;
}

/*!
Sets the download file checksum
\sa Update::setChecksum
*/
QString Update::getChecksum() const
{
    return m_strChecksum;
}

/*!
Returns the algorithm of the download file checksum
\sa Update::getChecksumAlg
*/
void Update::setChecksumAlg(const QString& aChecksumAlg)
{
    m_strChecksumAlg = aChecksumAlg;
}

/*!
Sets the download file checksum algorithm
\sa Update::setChecksumAlg
*/
QString Update::getChecksumAlg() const
{
    return m_strChecksumAlg;
}

/*!
Returns the signature of the download file
\sa Update::getSignature
*/
void Update::setSignature(const QString& aSignature)
{
    m_strSignature = aSignature;
}

/*!
Sets the download file signature
\sa Update::setSignature
*/
QString Update::getSignature() const
{
    return m_strSignature;
}
