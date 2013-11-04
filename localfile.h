#ifndef LOCALFILE_H
#define LOCALFILE_H

#include <QString>

namespace UpdateNode
{
    class LocalFile
    {
        public:
            static QString getDownloadLocation(const QString& aFileName);
            static QString getDownloadPath();
            static QString getCachePath();

    };
}
#endif // LOCALFILE_H
