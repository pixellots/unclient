/****************************************************************************
**
** Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
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

#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QTimer>
#include <QSharedMemory>
#include <QTranslator>

namespace UpdateNode
{
    class Application : public QObject
    {
        Q_OBJECT

        public:
            explicit Application(QObject *parent = 0);

            bool relaunchUpdateSave(const QString& aKey);
            bool relaunch(const QString& aKey);
            bool isAlreadyRunning(const QString& aKey);
            bool isHidden();
            void killOther();

            bool installTranslations();

        public slots:
            void setVisible(bool aShown = true);
            void killMeOrNot();


        private:
            QTranslator m_oTranslator;
            QSharedMemory m_oSharedMemory;
            QTimer m_oTimer;
            bool m_visible;

    };
}
#endif // SINGLEAPPLICATION_H
