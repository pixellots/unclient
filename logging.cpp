#include "logging.h"
#include "config.h"

using namespace UpdateNode;

Logging::Logging()
{
    m_pFile = NULL;
    m_pStream = NULL;

    if(Config::Instance()->isLoggingEnabled())
    {
        m_pFile = new QFile(Config::Instance()->getLoggingFile());
        if(m_pFile->open(QIODevice::WriteOnly | QIODevice::Append))
            m_pStream = new QTextStream(m_pFile);
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
