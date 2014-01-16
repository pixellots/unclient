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
    ../src/update.cpp \
    ../src/xmlparser.cpp \
    ../src/wincommander.cpp \
    ../src/version.cpp \
    tst_clienttest.cpp \
    ../src/logging.cpp \
    ../src/updatenode_service.cpp

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
    ../inc/update.h \
    ../inc/xmlparser.h \
    ../inc/wincommander.h \
    ../inc/version.h \
    ../inc/logging.h \
    ../inc/updatenode_service.h

macx:SOURCES += ../src/maccommander.cpp
macx:HEADERS += ../inc/maccommander.h

win32{
LIBS+= Shell32.lib Advapi32.lib
}

unix:QMAKE_POST_LINK += ./$$TARGET
win32:QMAKE_POST_LINK += $${TARGET}.exe
