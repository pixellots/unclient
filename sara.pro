QT = network \
    xml \
    gui \
    core \
    webkit

TARGET = unclient
TEMPLATE = app

DESTDIR = $$(PWD)

TRANSLATIONS = \
    translations/en_US.ts \
    translations/de_DE.ts \
    translations/ja_JP.ts \
    translations/zh_CN.ts \
    translations/es_ES.ts \
    translations/ru_RU.ts \
    translations/fr_FR.ts


#CONFIG-=app_bundle

RC_FILE = unclient.rc

VERSION_HIGH=1
VERSION_LOW=0
VERSION_REV=0
VERSION_BUILD=0

VERSION=1.0

QMAKE_TARGET_COMPANY = UpdateNode
QMAKE_TARGET_PRODUCT ="UpdateNode Client"
QMAKE_TARGET_DESCRIPTION = Client for software updates and messages
QMAKE_TARGET_COPYRIGHT =©2013 UpdateNode UG. All rights reserved.

DEFINES += \
APP_VERSION=$$VERSION \
APP_VERSION_HIGH=$$VERSION_HIGH \
APP_VERSION_LOW=$$VERSION_LOW   \
APP_VERSION_REV=$$VERSION_REV   \
APP_VERSION_BUILD=$$VERSION_BUILD

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
    singleappdialog.ui \
    usernotofication.ui \
    usermessages.ui \
    multiappdialog.ui
RESOURCES += res.qrc

win32{
LIBS+= Shell32.lib Advapi32.lib
}

ICON += images/unclient.icns
#QMAKE_INFO_PLIST = Info.plist

QMAKE_PRE_LINK  += @echo ++++++QMAKE_PRE_LINK++++++
QMAKE_POST_LINK += @echo ++++++QMAKE_POST_LINK++++++

OTHER_FILES += \
    unclient.rc
