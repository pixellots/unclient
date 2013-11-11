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

#ifndef USERNOTOFICATION_H
#define USERNOTOFICATION_H

#include <QDialog>
#include <QUrl>

#include "update.h"

namespace Ui {
    class UserNotofication;
}

class UserNotofication : public QDialog {
    Q_OBJECT
public:
    UserNotofication(QWidget *parent = 0);
    ~UserNotofication();

public:
    void updateView();

public slots:
    void openLink(const QUrl& aUrl);
    void updateSelectedUpdate();
    void detailsClicked();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::UserNotofication *ui;
};

#endif // USERNOTOFICATION_H
