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

#include <QStringList>

#include "version.h"

using namespace UpdateNode;

/*!
\class UpdateNode::Version
\brief Class container for comparing two version strings
*/

/*!
Compares two version strings and returns 0 when both versions match. -1 when first paramenter version is newer, 1 when second parameter is newer
\code
UpdateNode::Version::compare("2.0", "1.0.0") // returns -1
UpdateNode::Version::compare("2.0.0.0.1", "3.0") // returns 1
UpdateNode::Version::compare("1.0", "1.0.0") // returns 0
\endcode
*/
int Version::compare(const QString &aVersionA, const QString &aVersionB)
{
    QStringList versionListA = aVersionA.split(".");
    QStringList versionListB = aVersionB.split(".");
    int values = 0;

    if(versionListA.size() < versionListB.size())
        values = versionListB.size();
    else
        values = versionListA.size();

    for(int i = 0; i < values; i++)
    {
        if(i >= versionListA.size()
            || i >= versionListB.size())
        {
            if(i >= versionListA.size())
                if(versionListB.at(i).toInt()!=0)
                    return 1;
            if(i >= versionListB.size())
                if(versionListA.at(i).toInt()!=0)
                    return -1;
        }
        else
        {
            if(versionListA.at(i).toInt() > versionListB.at(i).toInt())
                return -1;
            else if(versionListA.at(i).toInt() < versionListB.at(i).toInt())
                return 1;
        }
    }
    return 0;
}
