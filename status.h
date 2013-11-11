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

#ifndef STATUS_H
#define STATUS_H

#define UPDATENODE_PROCERROR_START                  100
#define UPDATENODE_PROCERROR_NO_UPDATES             UPDATENODE_PROCERROR_START + 1
#define UPDATENODE_PROCERROR_WRONG_PARAMETER        UPDATENODE_PROCERROR_START + 2
#define UPDATENODE_PROCERROR_CANCELED               UPDATENODE_PROCERROR_START + 3
#define UPDATENODE_PROCERROR_UPDATE_EXEC_FAILED     UPDATENODE_PROCERROR_START + 4
#define UPDATENODE_PROCERROR_UPDATE_EXEC_CRASHED    UPDATENODE_PROCERROR_START + 5
#define UPDATENODE_PROCERROR_COMMAND_LAUNCH_FAILED  UPDATENODE_PROCERROR_START + 6
#define UPDATENODE_PROCERROR_DOWNLOAD_FAILED        UPDATENODE_PROCERROR_START + 7
#define UPDATENODE_PROCERROR_WINDOWS_UAC_FAILED     UPDATENODE_PROCERROR_START + 8
#define UPDATENODE_PROCERROR_RUN_DOWNLOAD_FIRST     UPDATENODE_PROCERROR_START + 9
#define UPDATENODE_PROCERROR_REGISTER_FIRST         UPDATENODE_PROCERROR_START + 10

#endif // STATUS_H
