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

#ifndef OSDETECTION_H
#define OSDETECTION_H

#include <QString>

namespace UpdateNode {

    class OSDetection
    {
        public:
            static QString getArch();
            static QString getOS();

            static QString getWindowsVersion();
            static QString getMacVersion();
            static QString getLinuxVersion();
            static QString getOthersVersion();

    };
}

#endif // OSDETECTION_H
