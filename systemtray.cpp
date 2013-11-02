#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QDebug>

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
