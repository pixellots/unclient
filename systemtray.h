#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QMenu>
#include <QSystemTrayIcon>

namespace UpdateNode
{
    class SystemTray : public QObject
    {
        Q_OBJECT

        public:
            explicit SystemTray(QObject *parent = 0);

            void showMessage(const QString& aText);
            void hide();
        signals:
            void launchClient();

        public slots:
            void onActivatedactivated(QSystemTrayIcon::ActivationReason aReason);

        private:
            QSystemTrayIcon m_oSystemTray;
            QMenu           m_oMenu;

    };
}

#endif // SYSTEMTRAY_H
