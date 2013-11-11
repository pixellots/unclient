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

#include <QLocale>
#include <stdlib.h>
#include "config.h"

using namespace UpdateNode;

/*!
\class UpdateNode::Config
\brief The QCursor class provides a mouse cursor with an arbitrary
shape.
\n
This class is mainly used to create mouse cursors that are
associated with particular widgets and to get and set the position
of the mouse cursor.
Qt has a number of standard cursor shapes, but you can also make
custom cursor shapes based on a QBitmap, a mask and a hotspot.
To associate a cursor with a widget, use QWidget::setCursor(). To
associate a cursor with all widgets (normally for a short period
of time), use QGuiApplication::setOverrideCursor().
To set a cursor shape use QCursor::setShape() or use the QCursor
constructor which takes the shape as argument, or you can use one
of the predefined cursors defined in the \l Qt::CursorShape enum.
If you want to create a cursor with your own bitmap, either use
the QCursor constructor which takes a bitmap and a mask or the
constructor which takes a pixmap as arguments.
To set or get the position of the mouse cursor use the static
methods QCursor::pos() and QCursor::setPos().
\b{Note:} It is possible to create a QCursor before
QGuiApplication, but it is not useful except as a place-holder for a
real QCursor created after QGuiApplication. Attempting to use a
QCursor that was created before QGuiApplication will result in a
crash.
*/

Config* Config::m_pInstance = NULL;

/*!
Constructs a custom pixmap cursor.
\a pixmap is the image. It is usual to give it a mask (set using
QPixmap::setMask()). \a hotX and \a hotY define the cursor's hot
spot.
\code
QString key = UpdateNode::Config::Instance()->getKey();
\endcode
If \a hotX is negative, it is set to the \c{pixmap().width()/2}.
If \a hotY is negative, it is set to the \c{pixmap().height()/2}.
Valid cursor sizes depend on the display hardware (or the
underlying window system). We recommend using 32 x 32 cursors,
because this size is supported on all platforms. Some platforms
also support 16 x 16, 48 x 48, and 64 x 64 cursors.
\note On Windows CE, the cursor size is fixed. If the pixmap
is bigger than the system size, it will be scaled.
\sa QPixmap::QPixmap(), QPixmap::setMask()
*/

Config* Config::Instance()
{
   if(!m_pInstance)
       m_pInstance = new Config;

   return m_pInstance;
}

Config::Config()
{
    m_bSilent = false;
    m_bSytemTray = false;
    m_bSingleMode = false;
    m_bRelaunch = false;
}

void Config::setKey(const QString& aKey)
{
    m_strKey = aKey;
}

QString Config::getKey() const
{
    return m_strKey;
}

void Config::setTestKey(const QString& aTestKey)
{
    m_strTestKey = aTestKey;
}

QString Config::getTestKey() const
{
    return m_strTestKey;
}

void Config::setVersionCode(const QString& aVersionCode)
{
    m_strVersionCode = aVersionCode;
}

QString Config::getVersionCode() const
{
    return m_strVersionCode;
}

void Config::setProductCode(const QString& aProductCode)
{
    m_strProductCode = aProductCode;
}

QString Config::getProductCode() const
{
    return m_strProductCode;
}

void Config::setVersion(const QString& aVersion)
{
    m_strVersion = aVersion;
}

QString Config::getVersion() const
{
    return m_strVersion;
}

void Config::setSilent(bool aSilent)
{
    m_bSilent = aSilent;
}

bool Config::isSilent()
{
    return m_bSilent;
}

QString Config::getOS() const
{
#ifdef Q_OS_UNIX
    return "Linux";
#else
#ifdef Win32
    return "Windows";
#else
    return "Mac";
#endif
#endif
}

UpdateNode::Product Config::product()
{
    return m_oProduct;
}

UpdateNode::ProductVersion Config::version()
{
    return m_oCurrentVersion;
}

QList<UpdateNode::Update> Config::updates()
{
    return m_listUpdates;
}

QList<UpdateNode::Message> Config::messages()
{
    return m_listMessages;
}

void Config::addUpdate(const UpdateNode::Update& aUpdate)
{
    m_listUpdates.append(aUpdate);
}

void Config::addMessage(const UpdateNode::Message& aMessage)
{
    m_listMessages.append(aMessage);
}

void Config::setProduct(const Product& aProduct)
{
    m_oProduct = aProduct;
}

void Config::setVersion(const ProductVersion& aVersion)
{
    m_oCurrentVersion = aVersion;
}

QString Config::mainIcon() const
{
    return m_strMainIcon;
}

void Config::setMainIcon(const QString& aIcon)
{
    m_strMainIcon = aIcon;
}

void Config::setLanguage(const QString& aLanguage)
{
    m_strLanguage = aLanguage;
}

QString Config::getLanguage() const
{
    return m_strLanguage.isEmpty() ? QLocale::system().name() : m_strLanguage;
}

void Config::clear()
{
    m_listUpdates.clear();
    m_listMessages.clear();
    m_listConfigs.clear();
}

void Config::setSystemTray(bool aTray)
{
    m_bSytemTray = aTray;
}

bool Config::isSystemTray()
{
    return m_bSytemTray;
}

void Config::setSingleMode(bool aSingleMode)
{
    m_bSingleMode = aSingleMode;
}

bool Config::isSingleMode()
{
    return m_bSingleMode;
}

void Config::setHost(const QString& aHost)
{
    m_strHost = aHost;
}

QString Config::getHost() const
{
    return m_strHost;
}

void Config::addConfiguration(UpdateNode::Config* aConfig)
{
    m_listConfigs.append(aConfig);
}

QList<UpdateNode::Config*> Config::configurations()
{
    return m_listConfigs;
}

void Config::clearConfigurations()
{
    m_listConfigs.clear();
}

void Config::setLogging(const QString &aFileName)
{
    m_strLogging = aFileName;
}

bool Config::isLoggingEnabled()
{
    return !m_strLogging.isEmpty();
}

QString Config::getLoggingFile()
{
    return m_strLogging;
}

void Config::setExec(const QString& aFileName)
{
    m_strExec = aFileName;
}

QString Config::getExec()
{
    return m_strExec;
}

void Config::setRelaunch(bool aEnable)
{
    m_bRelaunch = aEnable;
}

bool Config::isRelaunch()
{
    return m_bRelaunch;
}

void Config::setSplashScreen(const QString& aFileName)
{
    m_strSplashImage = aFileName;
}

QString Config::getSplashScreen()
{
    return m_strSplashImage;
}





