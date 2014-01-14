#ifndef MACCOMMANDER_H
#define MACCOMMANDER_H

#include <QString>
#include <QStringList>

namespace UpdateNode
{
    class MacCommander
    {

    public:
        static bool isProcessElevated();
        static uint runProcessElevated(const QString &path,
                                       const QStringList &parameters = QStringList(),
                                       const QString &description = QString(),
                                       const QString &icon = QString());
    };
}

#endif // MACCOMMANDER_H
