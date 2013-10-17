#ifndef VERSION_H
#define VERSION_H

#include <QString>

#define UPDATENODE_CLIENT_VERSION "1.0"

namespace UpdateNode
{
    class Version
    {
        public:
            static int compare(const QString& aVersionA, const QString& aVersionB);
    };
}

#endif // VERSION_H
