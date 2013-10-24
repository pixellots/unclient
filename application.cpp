#include "application.h"
#include <QApplication>
#include <QDebug>

using namespace UpdateNode;

Application::Application(QObject *parent) :
    QObject(parent)
{
}

bool Application::isAlreadyRunning(const QString& aKey)
{
    m_oSharedMemory.setKey(aKey);
    if (!m_oSharedMemory.create(1))
            return true;

    setSystemTrayShown(false);

    connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(killMeOrNot()));
    m_oTimer.start(1000);
    return false;
}

bool Application::isSystemTrayHidden()
{
    m_oSharedMemory.attach();

    m_oSharedMemory.lock();

    char* data = (char*)m_oSharedMemory.data();

    if(data)
    {
        if(data[0] == 'H')
        {
            m_oSharedMemory.unlock();
            return true;
        }
    }
    m_oSharedMemory.unlock();
    return false;
}

void Application::setSystemTrayShown(bool aShown)
{
    m_oSharedMemory.lock();
    char* data = (char*)m_oSharedMemory.data();

    if(data)
    {
        if(aShown)
            qMemCopy(data, "S", 1);
        else
            qMemCopy(data, "H", 1);
    }

    m_oSharedMemory.unlock();
}

void Application::killOther()
{
    m_oSharedMemory.lock();

    char* data = (char*)m_oSharedMemory.data();

    if(data)
        qMemCopy(data, "-", 1);

    m_oSharedMemory.unlock();
}

void Application::killMeOrNot()
{
    m_oSharedMemory.lock();

    char* data = (char*)m_oSharedMemory.data();

    if(data)
    {
        if(data[0] == '-')
        {
            qMemCopy(data, "H", 1);
            qApp->quit();
        }
    }
    m_oSharedMemory.unlock();
}
