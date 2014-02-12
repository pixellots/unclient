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

#ifndef USERMESSAGES_H
#define USERMESSAGES_H

#include <QDialog>
#include <QList>
#include <QUrl>

#include "message.h"
#include "updatenode_service.h"

namespace Ui {
#ifdef QT_WEBKIT_LIB
class UserMessagesEx;
#else
class UserMessages;
#endif
}

class UserMessages : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserMessages(QWidget *parent = 0);
    ~UserMessages();
    
    void init(UpdateNode::Service* aService);

public slots:
    void serviceDone();
    void onLeft();
    void onRight();
    void onRead();
    void onClose();
    void openLink(const QUrl&);

private:
    void showMessage();

private:
#ifdef QT_WEBKIT_LIB
    Ui::UserMessagesEx *ui;
#else
    Ui::UserMessages *ui;
#endif
	bool m_bFromRight;
    UpdateNode::Service* m_pService;
    QList<UpdateNode::Message> m_listMessages;

    int m_iCurrentIndex;
};

#endif // USERMESSAGES_H
