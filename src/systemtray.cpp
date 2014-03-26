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

#include <QApplication>
#include <QMenu>
#include <QAction>
#include "logging.h"

#include "config.h"
#include "systemtray.h"
#include "updatenode_service.h"

using namespace UpdateNode;

/*!
\class SystemTray
\brief Class responsible for system tray related activities.
*/

SystemTray::SystemTray(QObject *parent) :
    QObject(parent)
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    m_oSystemTray.setContextMenu(&m_oMenu);

    connect(&m_oSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivatedactivated(QSystemTrayIcon::ActivationReason)));

    if(config->isSingleMode())
        m_oSystemTray.setToolTip(QString("%1 %2").arg(config->product().getName()).arg(config->getVersion()));

    if(config->mainIcon().isEmpty() && config->isSingleMode())
        m_oSystemTray.setIcon(QIcon(config->product().getLocalIcon()));
    else if(!config->mainIcon().isEmpty())
        m_oSystemTray.setIcon(QIcon(config->mainIcon()));
    else
        m_oSystemTray.setIcon(QIcon(":/images/updatenode.png"));

    m_pUpdateAction = m_oMenu.addAction(QObject::tr("Launch Update Client"));

    QObject::connect(&m_oSystemTray, SIGNAL(messageClicked()), this, SIGNAL(launchClient()));
    QObject::connect(m_pUpdateAction, SIGNAL(triggered()), this, SIGNAL(launchClient()));

    if(config->isSingleMode())
    {
        m_pMessageAction = m_oMenu.addAction(QObject::tr("Read Messages"));
        QObject::connect(m_pMessageAction, SIGNAL(triggered()), this, SIGNAL(launchMessages()));
    }
    else
        m_pMessageAction = NULL;

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

void SystemTray::actionsBasedOnReturn(int aRetrunCode)
{
    switch(aRetrunCode)
    {
        case UpdateNode::Service::NOTHING:
            m_pUpdateAction->setDisabled(true);
            if(m_pMessageAction)
                m_pMessageAction->setDisabled(true);
            break;
        case UpdateNode::Service::UPDATE:
            m_pUpdateAction->setDisabled(false);
            if(m_pMessageAction)
                m_pMessageAction->setDisabled(true);
            break;
        case UpdateNode::Service::MESSAGE:
            m_pUpdateAction->setDisabled(true);
            if(m_pMessageAction)
                m_pMessageAction->setDisabled(false);
            break;
        case UpdateNode::Service::UPDATE_MESSAGE:
            m_pUpdateAction->setDisabled(false);
            if(m_pMessageAction)
                m_pMessageAction->setDisabled(false);
            break;
        default:
            m_pUpdateAction->setDisabled(true);
            m_pMessageAction->setDisabled(true);
            break;
    }
}
