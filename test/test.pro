#-------------------------------------------------
#
# Project created by QtCreator 2013-10-11T21:11:47
#
#-------------------------------------------------

QT       += network xml testlib
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = ./
TEMPLATE = app

INCLUDEPATH += ../inc

DEFINES += \
    APP_VERSION=0.0 \
    APP_VERSION_HIGH=0 \
    APP_VERSION_LOW=0   \
    APP_VERSION_REV=0   \
    APP_VERSION_BUILD=0

DEFINES += UNITTEST

SOURCES += \
    ../src/commander.cpp \
    ../src/config.cpp \
    ../src/localfile.cpp \
    ../src/downloader.cpp \
    ../src/productversion.cpp \
    ../src/product.cpp \
    ../src/osdetection.cpp \
    ../src/message.cpp \
    ../src/settings.cpp \
	../src/binarysettings.cpp \
    ../src/update.cpp \
    ../src/xmlparser.cpp \
    ../src/wincommander.cpp \
    ../src/version.cpp \
    tst_clienttest.cpp \
    ../src/logging.cpp \
    ../src/updatenode_service.cpp \
    ../src/limittimer.cpp \
    ../src/security.cpp

DEFINES += SRCDIR=../src

HEADERS += \
    ../inc/commander.h \
    ../inc/config.h \
    ../inc/localfile.h \
    ../inc/downloader.h \
    ../inc/productversion.h \
    ../inc/product.h \
    ../inc/osdetection.h \
    ../inc/message.h \
    ../inc/settings.h \
	../inc/binarysettings.h \
    ../inc/update.h \
    ../inc/xmlparser.h \
    ../inc/wincommander.h \
    ../inc/version.h \
    ../inc/logging.h \
    ../inc/updatenode_service.h \
    ../inc/limittimer.h \
    ../inc/security.h

macx:SOURCES += ../src/maccommander.cpp
macx:HEADERS += ../inc/maccommander.h

win32{
INCLUDEPATH += C:/OpenSSL-Win32/include
LIBS+= -L"C:/OpenSSL-Win32/lib" -llibeay32
LIBS+= User32.lib Gdi32.lib
}

win32{
LIBS+= Shell32.lib Advapi32.lib Netapi32.lib
}

macx{
CONFIG-=app_bundle
LIBS += -framework CoreFoundation
LIBS += -framework Security
}

unix:QMAKE_POST_LINK += ./$$TARGET
win32:QMAKE_POST_LINK += $${TARGET}.exe

RESOURCES += ../signature.qrc
