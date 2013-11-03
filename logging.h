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
