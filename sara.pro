# -------------------------------------------------
# Project created by QtCreator 2013-01-11T15:01:11
# -------------------------------------------------
QT = network \
    xml \
    gui \
    core \
    webkit
TARGET = unclient
TEMPLATE = app
TRANSLATIONS = translations/en_US.ts translations/de_DE.ts
CONFIG-=app_bundle

SOURCES += main.cpp \
    config.cpp \
    settings.cpp \
    xmlparser.cpp \
    product.cpp \
    productversion.cpp \
    update.cpp \
    message.cpp \
    downloader.cpp \
    osdetection.cpp \
    commander.cpp \
    localfile.cpp \
    singleappdialog.cpp \
    usernotofication.cpp \
    usermessages.cpp \
    updatenode_service.cpp \
    wincommander.cpp \
    version.cpp \
    application.cpp \
    systemtray.cpp \
    multiappdialog.cpp \
    logging.cpp
HEADERS += \
    config.h \
    settings.h \
    version.h \
    xmlparser.h \
    product.h \
    productversion.h \
    update.h \
    message.h \
    downloader.h \
    osdetection.h \
    commander.h \
    localfile.h \
    singleappdialog.h \
    status.h \
    usernotofication.h \
    usermessages.h \
    updatenode_service.h \
    wincommander.h \
    application.h \
    systemtray.h \
    multiappdialog.h \
    logging.h
FORMS += \
    settingsdialog.ui \
    singleappdialog.ui \
    usernotofication.ui \
    usermessages.ui \
    multiappdialog.ui
RESOURCES += res.qrc

win32{
LIBS+= Shell32.lib Advapi32.lib
}
