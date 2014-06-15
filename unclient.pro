##############################################################################
##
## Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
## Contact: code@updatenode.com
##
## This file is part of the UpdateNode Client.
##
## Commercial License Usage
## Licensees holding valid commercial UpdateNode license may use this file
## under the terms of the the Apache License, Version 2.0
## Full license description file: LICENSE.COM
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3.0 as published by the Free Software
## Foundation. Please review the following information to ensure the
## GNU General Public License version 3.0 requirements will be met:
## http://www.gnu.org/copyleft/gpl.html.
## Full license description file: LICENSE.GPL
##
##############################################################################

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
static{
CONFIG+= static
}

SUBDIRS=test

### version needs to be checked here
VERSION_HIGH=1
VERSION_LOW=1
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
    translations/$${TARGET}_ru.ts \
	translations/$${TARGET}_ko.ts

### qmake settings - sometimes this is not the reality
QMAKE_TARGET_COMPANY = UpdateNode
QMAKE_TARGET_PRODUCT = UpdateNode Client
QMAKE_TARGET_DESCRIPTION = UpdateNode Client
QMAKE_TARGET_COPYRIGHT = Copyright by (C) UpdateNode UG. All rights reserved.

DEFINES += \
    APP_VERSION=\\\"$${VERSION}\\\" \
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
    src/wincommander.cpp \
    src/limittimer.cpp \
    src/binarysettings.cpp

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
    inc/wincommander.h \
    inc/limittimer.h \
    inc/binarysettings.h

FORMS += \
    forms/singleappdialog.ui \
    forms/usernotofication.ui \
    forms/usermessages.ui \
    forms/multiappdialog.ui \
    forms/helpdialog.ui \
    forms/usermessages_ex.ui

RESOURCES += res.qrc \
    translations.qrc \
    cert.qrc

win32{
### on Windows we need some additional libs for the UAC
LIBS+= Shell32.lib Advapi32.lib Netapi32.lib
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

DOLLAR= $
win32{
!static{

clean_package_deploy.commands = -cmd.exe /C $(DEL_DIR) /S/Q package
clean_package_deploy.target = clean_package_deploy

create_package_deploy.commands = $(MKDIR) package
create_package_deploy.target = create_package_deploy

greaterThan(QT_MAJOR_VERSION, 4){
qt_deploy.commands = windeployqt -xml -core -network -widgets -webkitwidgets --no-quick-import --no-translations --no-system-d3d-compiler --no-webkit2 --dir package release\\unclient.exe
qt_deploy.target = qt_deploy
}

lessThan(QT_MAJOR_VERSION, 5){
qtcore_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtcore4.dll" package
qtcore_deploy.target = qtcore_deploy
qtgui_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtgui4.dll" package
qtgui_deploy.target = qtgui_deploy
qtnetwork_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtnetwork4.dll" package
qtnetwork_deploy.target = qtnetwork_deploy
qtxml_deploy.commands = $(COPY) "%QTDIR%\\bin\\qtxml4.dll" package
qtxml_deploy.target = qtxml_deploy
}

ssl_package_deploy.commands = $(COPY) c:\\OpenSSL-Win32\\libeay32.dll package && $(COPY) c:\\OpenSSL-Win32\\ssleay32.dll package
ssl_package_deploy.target = ssl_package_deploy

lessThan(QT_MAJOR_VERSION, 5){
package_deploy.depends = clean_package_deploy create_package_deploy qtcore_deploy qtgui_deploy qtnetwork_deploy qtxml_deploy qtwebkit_deploy ssl_package_deploy copy_binary vc_deploy
}
greaterThan(QT_MAJOR_VERSION, 4){
package_deploy.depends = clean_package_deploy create_package_deploy qt_deploy ssl_package_deploy copy_binary vc_deploy
}
package_deploy.target = package_deploy

sign{
build_installer.commands = iscc installer\\setup.iss /f$${SETUP_NAME} \"/sSIGN=signtool sign /a /t http://timestamp.globalsign.com/scripts/timestamp.dll $${DOLLAR}$${DOLLAR}p\" /dSIGNED /dUNclientVersionFull=$${VERSION_FULL} /dUNclientVersion=$${VERSION}
}
!sign{
build_installer.commands = iscc installer\\setup.iss /f$${SETUP_NAME} /dUNclientVersionFull=$${VERSION_FULL} /dUNclientVersion=$${VERSION}
}
build_installer.target = build_installer

copy_binary.commands = $(COPY) release\\unclient.exe package
copy_binary.target = copy_binary

QMAKE_EXTRA_TARGETS += copy_binary build_installer package_deploy clean_package_deploy create_package_deploy qt_deploy qtcore_deploy qtgui_deploy qtnetwork_deploy qtxml_deploy qtwebkit_deploy ssl_package_deploy vc_deploy
}
}

unix{
package_targz.commands = tar czf unclient-$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV}.tar.gz * --exclude=.git --exclude=.gitignore --exclude=. --exclude=unclient-$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV}.tar.gz
package_targz.target = package_targz

package_prepare.commands = mkdir -p unclient-$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV} && cd unclient-$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV} && tar xvfz ../unclient-$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV}.tar.gz && dh_make --quilt -s -e support@updatenode.com -c gpl3 -f ../unclient-$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV}.tar.gz && $(COPY) ../deploy/linux/* debian/ && rm -fr debian/*.ex debian/*.EX debian/README.* && vi debian/changelog
package_prepare.target = package_prepare

package_build.commands = cd unclient-$${VERSION_HIGH}.$${VERSION_LOW}.$${VERSION_REV} && fakeroot dpkg-buildpackage -D
package_build.target = package_build

package_deploy.depends = package_targz package_prepare package_build
package_deploy.target = package_deploy

QMAKE_EXTRA_TARGETS += package_deploy package_targz package_prepare package_build
}

### every first qmake call and each call after deloy target does generate a fresh build number
!exists( build.no.temp ) {
win32::system("@set /a $$cat(build.no)+1  > build.no.temp && @copy /Y build.no.temp build.no > NUL")
unix::system("/bin/bash -c \"echo $[$$cat(build.no)+1] > build.no.temp && cp build.no.temp build.no\"")
message("INFO: The build number has been increased to $$cat(build.no)")
}

#### deploy target
#### to be used for creating a new release
unix::deploy.commands = rm build.no.temp && echo ***************** SUCCESSFULLY BUILD ******************
unix::deploy.depends = clean updateqm all

!static{
win32::deploy.commands = del build.no.temp && @echo ***************** SUCCESSFULLY BUILD ****************** && dir package
win32::deploy.depends = clean updateqm release package_deploy
}
static{
win32::deploy.commands = del build.no.temp && @echo ***************** SUCCESSFULLY BUILD ******************
win32::deploy.depends = clean updateqm release
}

deploy.target = deploy
QMAKE_EXTRA_TARGETS += deploy

macx{
bundle{
message("INFO: Building unclient as a bundle")
ICON += images/unclient.icns
QMAKE_POST_LINK += -sed -i -e \"s/Created\ by\ Qt\/QMake/$$VERSION_FULL/g\" \"./$${TARGET}.app/Contents/Info.plist\";
}
!bundle{
message("INFO: Building unclient as a single binary")
CONFIG-=app_bundle
}
LIBS += -framework CoreFoundation
LIBS += -framework Security
}
