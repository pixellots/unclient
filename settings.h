#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

#define SARA_COMPANY_STR        "Sara"
#define SARA_APPLICATION_STR    "Client"

namespace Sara
{
    class Settings : public QSettings
    {
        public:
            Settings();

        public:
            QString uuid();

        private:
            QString m_strUUID;
    };
}

#endif // SETTINGS_H
