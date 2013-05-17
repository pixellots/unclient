#ifndef LOCALFILE_H
#define LOCALFILE_H

#include <QString>

namespace Sara
{
    class LocalFile
    {
        public:
            static QString getDownloadLocation(const QString& aFileName);
            static QString getDownloadPath();

    };
}
#endif // LOCALFILE_H
