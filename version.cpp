#include <QStringList>

#include "version.h"

using namespace UpdateNode;

int Version::compare(const QString &aVersionA, const QString &aVersionB)
{
    QStringList versionListA = aVersionA.split(".");
    QStringList versionListB = aVersionB.split(".");
    int values = 0;

    if(versionListA.size() < versionListB.size())
        values = versionListB.size();
    else
        values = versionListA.size();

    for(int i = 0; i < values; i++)
    {
        if(i >= versionListA.size()
            || i >= versionListB.size())
        {
            if(i >= versionListA.size())
                if(versionListB.at(i).toInt()!=0)
                    return 1;
            if(i >= versionListB.size())
                if(versionListA.at(i).toInt()!=0)
                    return -1;
        }
        else
        {
            if(versionListA.at(i).toInt() > versionListB.at(i).toInt())
                return -1;
            else if(versionListA.at(i).toInt() < versionListB.at(i).toInt())
                return 1;
        }
    }
    return 0;
}
