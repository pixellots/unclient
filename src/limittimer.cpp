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

#include "limittimer.h"
#include "status.h"

#include <QCoreApplication>

using namespace UpdateNode;

/*!
\class UpdateNode::LimitTimer
\brief Sets and controls the time-out for a request to updatenode
*/

/*!
Global instance of the LimitTimer pointer
*/
LimitTimer* LimitTimer::m_pInstance = NULL;

/*!
Retrieves the global LimitTimer class instance as an pointer. If no instance is present,
a new one will be created in this method
*/
LimitTimer* LimitTimer::Instance()
{
   if(!m_pInstance)
       m_pInstance = new LimitTimer;

   return m_pInstance;
}

/*!
Constructs an empty LimitTimer and connects SIGNAL timeout to exit()
*/
LimitTimer::LimitTimer() :
    QTimer(0)
{
    connect(this, SIGNAL(timeout()), SLOT(exit()));
}

/*!
Exits the application with error UPDATENODE_PROCERROR_TIMEOUT_REACHED on timeout
*/
void LimitTimer::exit()
{
    qApp->exit(UPDATENODE_PROCERROR_TIMEOUT_REACHED);
}

