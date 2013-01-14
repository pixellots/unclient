# -------------------------------------------------
# Project created by QtCreator 2013-01-11T15:01:11
# -------------------------------------------------
QT = network \
    xml \
    gui \
    core
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
    downloader.cpp
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
    downloader.h
FORMS += dialog.ui
