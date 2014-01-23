QT = network \
    xml \
    gui \
    core 

### QT_WEBKIT_LIB
webkit{
lessThan(QT_MAJOR_VERSION, 5): QT += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets
}
!webkit{
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
}

### version needs to be checked here
VERSION_HIGH=1
VERSION_LOW=0
VERSION_REV=1
VERSION_BUILD=$$cat(build.no)

VERSION=$${VERSION_HIGH}.$${VERSION_LOW}
VERSION_FULL=$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV}.$${VERSION_BUILD}   

SETUP_NAME=setup_$${VERSION}
TARGET = unclient
TEMPLATE = app

TRANSLATIONS = \
    translations/$${TARGET}_de.ts \
    translations/$${TARGET}_ja.ts \
    translations/$${TARGET}_zh.ts \
    translations/$${TARGET}_es.ts \
    translations/$${TARGET}_ru.ts

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

INCLUDEPATH += inc gen/ui_inc
OBJECTS_DIR = gen/obj
MOC_DIR = gen/moc
RCC_DIR = gen/rcc
UI_DIR = gen/ui
UI_HEADERS_DIR = gen/ui_inc
UI_SOURCES_DIR = gen/ui_src

SOURCES += src/main.cpp \
    src/config.cpp \
    src/settings.cpp \
    src/xmlparser.cpp \
    src/product.cpp \
    src/productversion.cpp \
    src/update.cpp \
    src/message.cpp \
    src/downloader.cpp \
    src/osdetection.cpp \
    src/commander.cpp \
    src/localfile.cpp \
    src/singleappdialog.cpp \
    src/usernotofication.cpp \
    src/usermessages.cpp \
    src/updatenode_service.cpp \
    src/version.cpp \
    src/application.cpp \
    src/systemtray.cpp \
    src/multiappdialog.cpp \
    src/logging.cpp \
    src/helpdialog.cpp \
    src/textbrowser.cpp \
    src/wincommander.cpp

macx:SOURCES += src/maccommander.cpp
macx:HEADERS += inc/maccommander.h

HEADERS += \
    inc/config.h \
    inc/settings.h \
    inc/version.h \
    inc/xmlparser.h \
    inc/product.h \
    inc/productversion.h \
    inc/update.h \
    inc/message.h \
    inc/downloader.h \
    inc/osdetection.h \
    inc/commander.h \
    inc/localfile.h \
    inc/singleappdialog.h \
    inc/status.h \
    inc/usernotofication.h \
    inc/usermessages.h \
    inc/updatenode_service.h \
    inc/application.h \
    inc/systemtray.h \
    inc/multiappdialog.h \
    inc/logging.h \
    inc/helpdialog.h \
    inc/textbrowser.h \
    inc/wincommander.h

FORMS += \
    forms/singleappdialog.ui \
    forms/usernotofication.ui \
    forms/usermessages.ui \
    forms/multiappdialog.ui \
    forms/helpdialog.ui \
    forms/usermessages_ex.ui

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

### when deploying, always generate new qm files
updateqm.commands = lrelease unclient.pro
updateqm.target = updateqm
QMAKE_EXTRA_TARGETS += updateqm


webkit{
message("INFO: Webkit support enabled")
win32{
qtwebkit_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtwebkit4.dll" package
qtwebkit_deploy.target = qtwebkit_deploy
}
SETUP_NAME=$${SETUP_NAME}_webkit
}
!webkit{
win32{
qtwebkit_deploy.commands =
qtwebkit_deploy.target = qtwebkit_deploy
}
}

vcredist{
win32{
message("WARNING: Make sure you have vcredist_x86.exe in 3rdparty folder before running build_installer")
vc_deploy.commands = $(COPY) "3rdparty\\vcredist_x86.exe" package
vc_deploy.target = vc_deploy
}
SETUP_NAME=$${SETUP_NAME}_vcredist
}
!vcredist{
vc_deploy.commands = 
vc_deploy.target = vc_deploy
}

win32{
clean_package_deploy.commands = -cmd.exe /C $(DEL_DIR) /S/Q package
clean_package_deploy.target = clean_package_deploy
create_package_deploy.commands = $(MKDIR) package
create_package_deploy.target = create_package_deploy
qtcore_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtcore4.dll" package
qtcore_deploy.target = qtcore_deploy
qtgui_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtgui4.dll" package
qtgui_deploy.target = qtgui_deploy
qtnetwork_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtnetwork4.dll" package
qtnetwork_deploy.target = qtnetwork_deploy
qtxml_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtxml4.dll" package
qtxml_deploy.target = qtxml_deploy
ssl_package_deploy.commands = $(COPY) c:\\OpenSSL-Win32\\libeay32.dll package && $(COPY) c:\\OpenSSL-Win32\\ssleay32.dll package
ssl_package_deploy.target = ssl_package_deploy
package_deploy.depends = clean_package_deploy create_package_deploy qtcore_deploy qtgui_deploy qtnetwork_deploy qtxml_deploy qtwebkit_deploy ssl_package_deploy copy_binary vc_deploy
package_deploy.target = package_deploy
build_installer.commands = iscc installer\\setup.iss /f$${SETUP_NAME} /dMyAppVersionFull=$${VERSION_FULL} /dMyAppVersion=$${VERSION}
build_installer.target = build_installer
copy_binary.commands = $(COPY) release\\unclient.exe package
copy_binary.target = copy_binary
QMAKE_EXTRA_TARGETS += copy_binary build_installer package_deploy clean_package_deploy create_package_deploy qtcore_deploy qtgui_deploy qtnetwork_deploy qtxml_deploy qtwebkit_deploy ssl_package_deploy vc_deploy
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
message("INFO: The build number has been increased to $$cat(build.no)")
}

#### deploy target
#### to be used for creating a new release
win32::deploy.commands = del build.no.temp && @echo ***************** SUCCESSFULLY BUILD ****************** && dir package
unix::deploy.commands = rm build.no.temp && echo ***************** SUCCESSFULLY BUILD ******************
win32::deploy.depends = clean updateqm release package_deploy
unix::deploy.depends = clean updateqm all
deploy.target = deploy
QMAKE_EXTRA_TARGETS += deploy

macx{
bundle{
message("INFO: Building unclient as a bundle")
ICON += images/unclient.icns
}
!bundle{
message("INFO: Building unclient as a single binary")
CONFIG-=app_bundle
}
LIBS += -framework CoreFoundation
LIBS += -framework Security
}
