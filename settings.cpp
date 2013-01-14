#include <stdlib.h>
#include <QString>
#include <QSettings>
#include <QUuid>
#include "settings.h"

using namespace Sara;

Settings::Settings()
    : QSettings(SARA_COMPANY_STR, SARA_APPLICATION_STR, 0)
{
    m_strUUID = QString("uuid");
}

QString Settings::uuid()
{
    QString id = this->value(m_strUUID).toString();

    if(id==QString::null)
    {
        id = QUuid::createUuid().toString();
        this->setValue(m_strUUID, id);
    }

    return id;
}
