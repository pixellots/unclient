#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QTimer>
#include <QSharedMemory>

namespace UpdateNode
{
    class Application : public QObject
    {
        Q_OBJECT

        public:
            explicit Application(QObject *parent = 0);

            bool isAlreadyRunning(const QString& aKey);
            bool isSystemTrayHidden();
            void killOther();

        public slots:
            void setSystemTrayShown(bool aShown = true);
            void killMeOrNot();


        private:
            QSharedMemory m_oSharedMemory;
            QTimer m_oTimer;

    };
}
#endif // SINGLEAPPLICATION_H
