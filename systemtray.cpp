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

#include <QApplication>
#include <QMenu>
#include <QAction>
#include "logging.h"

#include "config.h"
#include "systemtray.h"

using namespace UpdateNode;

SystemTray::SystemTray(QObject *parent) :
    QObject(parent)
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    m_oSystemTray.setContextMenu(&m_oMenu);

    connect(&m_oSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivatedactivated(QSystemTrayIcon::ActivationReason)));

    if(config->isSingleMode())
        m_oSystemTray.setToolTip(QObject::tr("%1 %2").arg(config->product().getName()).arg(config->getVersion()));

    if(config->mainIcon().isEmpty() && config->isSingleMode())
        m_oSystemTray.setIcon(QIcon(config->product().getLocalIcon()));
    else if(!config->mainIcon().isEmpty())
        m_oSystemTray.setIcon(QIcon(config->mainIcon()));
    else
        m_oSystemTray.setIcon(QIcon(":/images/updatenode.png"));

    QAction* action = m_oMenu.addAction(QObject::tr("Launch Update Client"));

    QObject::connect(&m_oSystemTray, SIGNAL(messageClicked()), this, SIGNAL(launchClient()));
    QObject::connect(action, SIGNAL(triggered()), this, SIGNAL(launchClient()));
    m_oMenu.addSeparator();
    QObject::connect(m_oMenu.addAction(QObject::tr("Close")), SIGNAL(triggered()), qApp, SLOT(quit()));
}

void SystemTray::showMessage(const QString &aText)
{
    m_oSystemTray.show();

    if(UpdateNode::Config::Instance()->isSingleMode())
        m_oSystemTray.showMessage(UpdateNode::Config::Instance()->product().getName(), aText);
    else
        m_oSystemTray.showMessage(tr("Software Update Manager"), aText);
}

void SystemTray::onActivatedactivated(QSystemTrayIcon::ActivationReason aReason)
{
    if(aReason == QSystemTrayIcon::DoubleClick)
        emit launchClient();
}

void SystemTray::hide()
{
    m_oSystemTray.hide();
}
