#include "application.h"
#include <QApplication>
#include <QDebug>
#include <QThread>

using namespace UpdateNode;

Application::Application(QObject *parent) :
    QObject(parent)
{
}

bool Application::isAlreadyRunning(const QString& aKey)
{
    m_oSharedMemory.setKey(aKey);
    if (!m_oSharedMemory.create(1))
    {
        connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(killMeOrNot()));
        m_oTimer.start(500);

        m_oSharedMemory.attach();
        return true;
    }

    char* data = (char*)m_oSharedMemory.data();

    if(data)
        qMemCopy(data, "0", 1);

    connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(killMeOrNot()));
    m_oTimer.start(500);
    return false;
}

bool Application::isHidden()
{
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

void Application::setVisible(bool aShown)
{
    m_visible = aShown;
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
            if(!m_visible)
                qMemCopy(data, "H", 1);
            else
                qMemCopy(data, "S", 1);
            qApp->quit();
        }
        else
        {
            if(!m_visible)
                qMemCopy(data, "H", 1);
            else
                qMemCopy(data, "S", 1);
        }
    }
    m_oSharedMemory.unlock();
}
