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

#ifndef LOGGING_H
#define LOGGING_H

#include <QString>
#include <QFile>
#include <QTextStream>

namespace UpdateNode
{
    class Logging
    {
        public:
            Logging();
            ~Logging();

            Logging& operator<<(const QString & t);
            Logging& operator<<(int t);

        private:
            QFile*          m_pFile;
            QTextStream*    m_pStream;

    };
}
#endif // LOGGING_H
