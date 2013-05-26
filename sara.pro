# -------------------------------------------------
# Project created by QtCreator 2013-01-11T15:01:11
# -------------------------------------------------
QT = network \
    xml \
    gui \
    core \
    webkit
TARGET = sara
TEMPLATE = app
SOURCES += main.cpp \
    dialog.cpp \
    config.cpp \
    settings.cpp \
    sara_service.cpp \
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
    usernotofication.cpp
HEADERS += dialog.h \
    config.h \
    settings.h \
    sara_service.h \
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
    usernotofication.h
FORMS += dialog.ui \
    settingsdialog.ui \
    singleappdialog.ui \
    usernotofication.ui
RESOURCES += res.qrc
