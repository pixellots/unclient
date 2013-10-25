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
SOURCES += main.cpp \
    dialog.cpp \
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
    systemtray.cpp
HEADERS += dialog.h \
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
    systemtray.h
FORMS += dialog.ui \
    settingsdialog.ui \
    singleappdialog.ui \
    usernotofication.ui \
    usermessages.ui
RESOURCES += res.qrc

win32{
LIBS+= Shell32.lib Advapi32.lib
}
