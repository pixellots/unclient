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

#include "helpdialog.h"
#include "ui_helpdialog.h"

/*!
\class Helpdialog
\brief Displays the help page in a text browser. Mainly for Windows, as on other platforms\n
the help is displayed on stdout
*/

Helpdialog::Helpdialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint),
    ui(new Ui::Helpdialog)
{
    ui->setupUi(this);

    setWindowIcon(QPixmap(":/images/updatenode.png"));
}

Helpdialog::~Helpdialog()
{
    delete ui;
}

void Helpdialog::setText(const QString& aAppName, const QString &aText)
{
    setWindowTitle(aAppName);

    ui->textEdit->setText(aAppName + "\n\n" + aText);
}
