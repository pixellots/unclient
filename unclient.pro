QT = network \
    xml \
    gui \
    core 

### QT_WEBKIT_LIB
QT += webkit

TARGET = unclient
TEMPLATE = app
#CONFIG += release

TRANSLATIONS = \
    translations/$${TARGET}_de.ts \
    translations/$${TARGET}_ja.ts \
    translations/$${TARGET}_zh.ts \
    translations/$${TARGET}_es.ts \
    translations/$${TARGET}_ru.ts

### when deploying, always generate new qm files
updateqm.commands = lrelease unclient.pro
updateqm.target = updateqm
QMAKE_EXTRA_TARGETS += updateqm

win32{
create_package_deploy.commands = $(CHK_DIR_EXISTS) package $(MKDIR) package
create_package_deploy.target = create _package_deploy
qtcore_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtcore4.dll" package
qtcore_deploy.target = qtcore_deploy
qtgui_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtgui4.dll" package
qtgui_deploy.target = qtgui_deploy
qtnetwork_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtnetwork4.dll" package
qtnetwork_deploy.target = qtnetwork_deploy
qtxml_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtxml4.dll" package
qtxml_deploy.target = qtxml_deploy
qtwebkit_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtwebkit4.dll" package
qtwebkit_deploy.target = qtwebkit_deploy
ssl_package_deploy.commands = $(COPY) c:\\OpenSSL-Win32\\*.dll package
ssl_package_deploy.target = ssl_package_deploy
package_deploy.depends = create_package_deploy qtcore_deploy qtgui_deploy qtnetwork_deploy qtxml_deploy qtwebkit_deploy ssl_package_deploy copy_binary
package_deploy.target = package_deploy
build_installer.commands = devenv.com installer\\setup.sln /Rebuild SingleImage
build_installer.target = build_installer
copy_binary.commands = $(COPY) release\\unclient.exe package
copy_binary.target = copy_binary
QMAKE_EXTRA_TARGETS += copy_binary build_installer package_deploy create_package_deploy qtcore_deploy qtgui_deploy qtnetwork_deploy qtxml_deploy qtwebkit_deploy ssl_package_deploy
}
!win32{
package_deploy.commands = echo *** UNSUPPORTED ***
package_deploy.target = package_deploy
QMAKE_EXTRA_TARGETS += package_deploy
}

### every first qmake call and each call after deloy target does generate a fresh build number
!exists( build.no.temp ) {
win32::system("@set /a $$cat(build.no)+1  > build.no.temp && @copy /Y build.no.temp build.no > NUL")
unix::system("/bin/bash -c \"echo $[$$cat(build.no)+1] > build.no.temp && cp build.no.temp build.no\"")
message("The build number has been increased to $$cat(build.no)")
}

#### deploy target
#### to be used for creating a new release
win32::deploy.commands = del build.no.temp && @echo ***************** SUCCESSFULLY BUILD ****************** && dir package
unix::deploy.commands = rm build.no.temp && echo ***************** SUCCESSFULLY BUILD ****************** && ls -l package
win32::deploy.depends = clean updateqm release package_deploy
unix::deploy.depends = clean updateqm all
deploy.target = deploy
QMAKE_EXTRA_TARGETS += deploy

#### MAC: uncomment 'ICON += images/unclient.icns' 
####      and comment 'CONFIG-=app_bundle'
####      if you need a bundle instead of a single binary
#ICON += images/unclient.icns
macx:CONFIG-=app_bundle
macx:LIBS += -framework CoreFoundation
macx:LIBS += -framework Security

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
    version.cpp \
    application.cpp \
    systemtray.cpp \
    multiappdialog.cpp \
    logging.cpp \
    helpdialog.cpp \
    textbrowser.cpp

win32:SOURCES += wincommander.cpp
win32:HEADERS += wincommander.h
macx:SOURCES += maccommander.cpp
macx:HEADERS += maccommander.h

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
### do not elevate automatically
QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'asInvoker\'
CONFIG += embed_manifest_exe
}
