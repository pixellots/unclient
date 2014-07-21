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
#include "cookiejar.h"
#include "localfile.h"

#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QList>
#include <QNetworkCookie>

using namespace UpdateNode;

/*!
\class UpdateNode::CookieJar
\brief Cookie Class to load and store pernament cookies for webkit-based user messages
\i Inspired by https://github.com/TamCore/facebook-qt/blob/master/CookieJar.cpp
*/
CookieJar::CookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
}

/*!
Stores the cookie file into api-key specific download path. Session and secure cookies are ignored.
\sa CookieJar::load
*/
void CookieJar::save()
{
    QList<QNetworkCookie> cookieList = allCookies();

    if(cookieList.isEmpty())
        return;

    QFile *cookieFile = new QFile(UpdateNode::LocalFile::getDownloadPath() + "/cookies.dat");
    if(!cookieFile->open(QIODevice::WriteOnly))
        return;

    QDataStream cookieStream(cookieFile);
    QNetworkCookie cookie;

    int count = cookieList.count();
    cookieStream << count;
    for(int i = 0; i < count; i++)
    {
        cookie = allCookies().at(i);
        if(!cookie.isSessionCookie() && !cookie.isSecure())
            cookieStream << cookie.toRawForm();
    }
    cookieFile->close();
    delete cookieFile;
}

/*!
Setting the cookies as defined  in the cookie file from the api-key specific download path.
\sa CookieJar::save
*/
void CookieJar::load()
{
    QList<QNetworkCookie> restoredCookies;

    QFile *cookieFile = new QFile(UpdateNode::LocalFile::getDownloadPath() + "/cookies.dat");
    if(!cookieFile->open(QIODevice::ReadOnly))
        return;

    int count;
    QDataStream cookieStream(cookieFile);
    cookieStream >> count;

    for(int i = 0; i < count; i++)
    {
        QByteArray cookieData;
        cookieStream >> cookieData;
        const QList<QNetworkCookie> &cookieList = QNetworkCookie::parseCookies(cookieData);

        if(cookieList.isEmpty())
            continue;

        const QNetworkCookie &cookie = cookieList.at(0);

        if(!cookie.isSecure() && !cookie.isSessionCookie())
            restoredCookies.append(cookie);
    }

    setAllCookies(restoredCookies);
    cookieFile->close();
    delete cookieFile;
}
