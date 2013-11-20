QT = network \
    xml \
    gui \
    core 

### QT_WEBKIT_LIB
#QT += webkit

TARGET = unclient
TEMPLATE = app
#CONFIG += release

TRANSLATIONS = \
    translations/$${TARGET}_de.ts \
    translations/$${TARGET}_ja.ts \
    translations/$${TARGET}_zh.ts \
    translations/$${TARGET}_es.ts \
    translations/$${TARGET}_ru.ts \
    translations/$${TARGET}_fr.ts

### when deploying, always generate new qm files
updateqm.commands = lrelease unclient.pro
updateqm.target = updateqm
QMAKE_EXTRA_TARGETS += updateqm

### every first qmake call and each call after deloy target does generate a fresh build number
!exists( build.no.temp ) {
win32::system("@set /a $$cat(build.no)+1  > build.no.temp && @copy /Y build.no.temp build.no > NUL")
unix::system("/bin/bash -c \"echo $[$$cat(build.no)+1] > build.no.temp && cp build.no.temp build.no\"")
message("The build number has been increased to $$cat(build.no)")
}

#### deploy target
#### to be used for creating a new release
win32::deploy.commands = del /Y build.no.temp && @echo DONE!!!
unix::deploy.commands = rm build.no.temp && echo DONE!!!
win32::deploy.depends = clean updateqm release
unix::deploy.depends = clean updateqm all
deploy.target = deploy
QMAKE_EXTRA_TARGETS += deploy

#### MAC: uncomment 'ICON += images/unclient.icns' 
####      and comment 'CONFIG-=app_bundle'
####      if you need a bundle instead of a single binary
#ICON += images/unclient.icns
CONFIG-=app_bundle

### version needs to be checked here
VERSION_HIGH=1
VERSION_LOW=0
VERSION_REV=1
VERSION_BUILD=$$cat(build.no)
VERSION=1.0

### qmake settings - sometimes this is not the reality
QMAKE_TARGET_COMPANY = UpdateNode
QMAKE_TARGET_PRODUCT = "UpdateNode Client"
QMAKE_TARGET_DESCRIPTION = Client for software updates and messages
QMAKE_TARGET_COPYRIGHT = Copyright by (C) UpdateNode UG. All rights reserved.

DEFINES += \
    APP_VERSION=$${VERSION} \
    APP_VERSION_HIGH=$${VERSION_HIGH} \
    APP_VERSION_LOW=$${VERSION_LOW}   \
    APP_VERSION_REV=$${VERSION_REV}   \
    APP_VERSION_BUILD=$${VERSION_BUILD}

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
    logging.cpp \
    helpdialog.cpp \
    textbrowser.cpp

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
    logging.h \
    helpdialog.h \
    textbrowser.h

FORMS += \
    singleappdialog.ui \
    usernotofication.ui \
    usermessages.ui \
    multiappdialog.ui \
    helpdialog.ui \
    usermessages_ex.ui

RESOURCES += res.qrc \
    translations.qrc

win32{
### on Windows we need some additional libs for the UAC
LIBS+= Shell32.lib Advapi32.lib
### rc file for Windows and the above QMAKE settings does not work perfect
RC_FILE = unclient.rc
}
