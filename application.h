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

            bool relaunchUpdateSave(const QString& aKey);
            bool relaunch(const QString& aKey);
            bool isAlreadyRunning(const QString& aKey);
            bool isHidden();
            void killOther();

        public slots:
            void setVisible(bool aShown = true);
            void killMeOrNot();


        private:
            QSharedMemory m_oSharedMemory;
            QTimer m_oTimer;
            bool m_visible;

    };
}
#endif // SINGLEAPPLICATION_H
