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

#include "usermessages.h"
#include "config.h"
#include "settings.h"
#include "localfile.h"

#ifdef QT_WEBKIT_LIB
#include <QtWebKit>
#include "ui_usermessages_ex.h"
#else
#include <QTextBrowser>
#include "ui_usermessages.h"
#endif

#include <QNetworkDiskCache>
#include <QDesktopServices>

UserMessages::UserMessages(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
#ifdef QT_WEBKIT_LIB
    ui(new Ui::UserMessagesEx)
#else
    ui(new Ui::UserMessages)
#endif
{
    ui->setupUi(this);

    QNetworkDiskCache* cache = new QNetworkDiskCache(this);
    cache->setCacheDirectory(UpdateNode::LocalFile::getCachePath());
#ifdef QT_WEBKIT_LIB
    ui->webView->page()->networkAccessManager()->setCache(cache);
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));
#endif
    connect(ui->toolLeft, SIGNAL(clicked()), SLOT(onLeft()));
    connect(ui->toolRight, SIGNAL(clicked()), SLOT(onRight()));
    connect(ui->pshRead, SIGNAL(clicked()), SLOT(onRead()));

    ui->progressBar->hide();
}

UserMessages::~UserMessages()
{
#ifdef QT_WEBKIT_LIB
    delete ui->webView->page()->networkAccessManager()->cache();
#endif
    delete ui;
}

void UserMessages::init(UpdateNode::Service* aService)
{
    m_pService = aService;
    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));
}

void UserMessages::serviceDone()
{
    UpdateNode::Settings settings;
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    m_iCurrentIndex = -1;
    m_listMessages.clear();

    if(!config->product().getIconUrl().isEmpty())
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    else if(!config->mainIcon().isEmpty())
        setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));

    QList<UpdateNode::Message> message_list= config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
        if(!settings.messageShownAndLoaded(message_list.at(i).getCode()))
            m_listMessages << message_list.at(i);
    }

    if(m_listMessages.isEmpty())
        qApp->quit();
    else
    {
        if(m_listMessages.size()==1)
            setWindowTitle(config->product().getName() + tr(" - Message"));
        else
            setWindowTitle(config->product().getName() + tr(" - Messages"));
        show();
    }
    layout()->setSizeConstraint(QLayout::SetMinimumSize);

    showMessage();

    setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
}


void UserMessages::showMessage()
{
    if(m_listMessages.isEmpty())
    {
        hide();
        qApp->quit();
        return;
    }

    if(m_iCurrentIndex == -1)
        m_iCurrentIndex = 0;

    ui->pshRead->setDisabled(true);

    if(m_listMessages.size()>1)
        ui->label->setText(m_listMessages.at(m_iCurrentIndex).getTitle() +  tr(" (%1/%2)").arg(m_iCurrentIndex+1).arg(m_listMessages.size()));
    else
        ui->label->setText(m_listMessages.at(m_iCurrentIndex).getTitle());

#ifdef QT_WEBKIT_LIB
    if(!m_listMessages.at(m_iCurrentIndex).getMessage().isEmpty())
        ui->webView->setContent(m_listMessages.at(m_iCurrentIndex).getMessage().toUtf8());
    else
        ui->webView->load(m_listMessages.at(m_iCurrentIndex).getLink());
#else
    if(!m_listMessages.at(m_iCurrentIndex).getMessage().isEmpty())
        ui->textBrowser->setHtml(m_listMessages.at(m_iCurrentIndex).getMessage());
    else
        ui->textBrowser->loadHtml(m_listMessages.at(m_iCurrentIndex).getLink());
    ui->pshRead->setEnabled(true);
#endif
    ui->progressBar->setFormat(tr("Loading %p% ..."));

    if(m_listMessages.size()==m_iCurrentIndex+1)
        ui->toolRight->setDisabled(true);
    else
        ui->toolRight->setEnabled(true);

    if(m_iCurrentIndex == 0)
        ui->toolLeft->setDisabled(true);
    else
        ui->toolLeft->setEnabled(true);

    if(m_listMessages.size()==1)
    {
        ui->toolLeft->hide();
        ui->toolRight->hide();
        ui->pshRead->setText(tr("Mark as read and close"));
    }
}

void UserMessages::onLeft()
{
    m_iCurrentIndex--;
    showMessage();
}

void UserMessages::onRight()
{
    m_iCurrentIndex++;
    showMessage();
}

void UserMessages::onRead()
{
    UpdateNode::Settings settings;
    UpdateNode::Message message = m_listMessages.takeAt(m_iCurrentIndex);

    settings.setMessage(message, true, true);

    m_iCurrentIndex = -1;
    showMessage();
}

void UserMessages::openLink(const QUrl& aUrl)
{
    QDesktopServices::openUrl(aUrl);
}

