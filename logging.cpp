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

#include "logging.h"
#include "config.h"

using namespace UpdateNode;

Logging::Logging()
{
    m_pFile = NULL;
    m_pStream = NULL;

    if(Config::Instance()->isLoggingEnabled())
    {
        if(Config::Instance()->getLoggingFile()=="-")
        {
            m_pFile = new QFile();
            if(m_pFile->open(stderr, QIODevice::WriteOnly))
            {
                m_pStream = new QTextStream(m_pFile);
                *m_pStream << "LOG: ";
            }
        }
        else
        {
            m_pFile = new QFile(Config::Instance()->getLoggingFile());
            if(m_pFile->open(QIODevice::WriteOnly | QIODevice::Append))
            {
                m_pStream = new QTextStream(m_pFile);
                *m_pStream << "LOG: ";
            }
        }
    }
}

Logging::~Logging()
{
    if(m_pFile)
    {
        *m_pStream << "\n";
        m_pFile->close();
        delete m_pStream;
        delete m_pFile;
    }
}

Logging& Logging::operator <<(const QString & t)
{
    if(m_pFile && m_pStream)
    {
        *m_pStream << t << " ";
    }
    return *this;
}

Logging& Logging::operator<<(int t)
{
    if(m_pFile && m_pStream)
    {
        *m_pStream << t << " ";
    }
    return *this;
}
