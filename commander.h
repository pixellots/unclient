#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>
#include <QProcess>
#include "update.h"

namespace UpdateNode
{
    class Commander : public QObject
    {
        Q_OBJECT
        public:
            Commander(QObject *parent = 0);

            bool run(const UpdateNode::Update& aUpdate);

            QString readStdErr() const;
            QString readStdOut() const;

        signals:
            void processError();
            void processOutput();
            void updateExit(int aExitCode, QProcess::ExitStatus aExitStatus);
            void progressText(const QString& aStatusText);

        private:
            QString setCommandBasedOnOS() const;
            QString resolve(const QString& aString);

        private:
            QProcess* m_pProcess;
            UpdateNode::Update m_oUpdate;
            bool m_bCopy;
    };
}

#endif // COMMANDER_H
