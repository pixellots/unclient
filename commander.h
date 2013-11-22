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
            ~Commander();

            bool run(const UpdateNode::Update& aUpdate);

            QString readStdErr() const;
            QString readStdOut() const;

            void setUpdate(const UpdateNode::Update& aUpdate);
            QString resolve(const QString& aString);

            bool waitForFinished();
            int getReturnCode();

        public:
            static QString resolveGeneral(const QString& aString);
            static bool copy(const QString& aFrom, const QString& aTo);
            static QStringList splitCommandLine(const QString& aString);

        signals:
            void processError();
            void processOutput();
            void updateExit(int aExitCode, QProcess::ExitStatus aExitStatus);
            void progressText(const QString& aStatusText);

        private:
            QString setCommandBasedOnOS() const;
            bool isProcessElevated();


        private:
            QProcess* m_pProcess;
            UpdateNode::Update m_oUpdate;
            bool m_bCopy;
    };
}

#endif // COMMANDER_H
