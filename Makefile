#############################################################################
# Makefile for building: sara
# Generated by qmake (2.01a) (Qt 4.7.4) on: Fri May 17 13:26:19 2013
# Project:  sara.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile sara.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtWebKit -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtWebKit -lQtXml -lQtGui -lQtNetwork -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
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
		localfile.cpp moc_dialog.cpp \
		moc_sara_service.cpp \
		moc_xmlparser.cpp \
		moc_downloader.cpp \
		moc_commander.cpp
OBJECTS       = main.o \
		dialog.o \
		config.o \
		settings.o \
		sara_service.o \
		xmlparser.o \
		product.o \
		productversion.o \
		update.o \
		message.o \
		downloader.o \
		osdetection.o \
		commander.o \
		localfile.o \
		moc_dialog.o \
		moc_sara_service.o \
		moc_xmlparser.o \
		moc_downloader.o \
		moc_commander.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		sara.pro
QMAKE_TARGET  = sara
DESTDIR       = 
TARGET        = sara

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_dialog.h ui_settingsdialog.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: sara.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtWebKit.prl \
		/usr/lib/i386-linux-gnu/libQtXml.prl \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtNetwork.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile sara.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtWebKit.prl:
/usr/lib/i386-linux-gnu/libQtXml.prl:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtNetwork.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile sara.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/sara1.0.0 || $(MKDIR) .tmp/sara1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/sara1.0.0/ && $(COPY_FILE) --parents dialog.h config.h settings.h sara_service.h version.h xmlparser.h product.h productversion.h update.h message.h downloader.h osdetection.h commander.h localfile.h .tmp/sara1.0.0/ && $(COPY_FILE) --parents main.cpp dialog.cpp config.cpp settings.cpp sara_service.cpp xmlparser.cpp product.cpp productversion.cpp update.cpp message.cpp downloader.cpp osdetection.cpp commander.cpp localfile.cpp .tmp/sara1.0.0/ && $(COPY_FILE) --parents dialog.ui settingsdialog.ui .tmp/sara1.0.0/ && (cd `dirname .tmp/sara1.0.0` && $(TAR) sara1.0.0.tar sara1.0.0 && $(COMPRESS) sara1.0.0.tar) && $(MOVE) `dirname .tmp/sara1.0.0`/sara1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/sara1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_dialog.cpp moc_sara_service.cpp moc_xmlparser.cpp moc_downloader.cpp moc_commander.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_dialog.cpp moc_sara_service.cpp moc_xmlparser.cpp moc_downloader.cpp moc_commander.cpp
moc_dialog.cpp: sara_service.h \
		downloader.h \
		update.h \
		productversion.h \
		commander.h \
		dialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) dialog.h -o moc_dialog.cpp

moc_sara_service.cpp: downloader.h \
		update.h \
		productversion.h \
		sara_service.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) sara_service.h -o moc_sara_service.cpp

moc_xmlparser.cpp: update.h \
		productversion.h \
		message.h \
		xmlparser.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) xmlparser.h -o moc_xmlparser.cpp

moc_downloader.cpp: update.h \
		productversion.h \
		downloader.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) downloader.h -o moc_downloader.cpp

moc_commander.cpp: update.h \
		productversion.h \
		commander.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) commander.h -o moc_commander.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_dialog.h ui_settingsdialog.h
compiler_uic_clean:
	-$(DEL_FILE) ui_dialog.h ui_settingsdialog.h
ui_dialog.h: dialog.ui
	/usr/bin/uic-qt4 dialog.ui -o ui_dialog.h

ui_settingsdialog.h: settingsdialog.ui
	/usr/bin/uic-qt4 settingsdialog.ui -o ui_settingsdialog.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp dialog.h \
		sara_service.h \
		downloader.h \
		update.h \
		productversion.h \
		commander.h \
		config.h \
		product.h \
		message.h \
		settings.h \
		version.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

dialog.o: dialog.cpp dialog.h \
		sara_service.h \
		downloader.h \
		update.h \
		productversion.h \
		commander.h \
		ui_dialog.h \
		message.h \
		product.h \
		config.h \
		localfile.h \
		settings.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dialog.o dialog.cpp

config.o: config.cpp config.h \
		product.h \
		productversion.h \
		update.h \
		message.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o config.o config.cpp

settings.o: settings.cpp settings.h \
		update.h \
		productversion.h \
		message.h \
		product.h \
		config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o settings.o settings.cpp

sara_service.o: sara_service.cpp sara_service.h \
		downloader.h \
		update.h \
		productversion.h \
		config.h \
		product.h \
		message.h \
		settings.h \
		version.h \
		xmlparser.h \
		osdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sara_service.o sara_service.cpp

xmlparser.o: xmlparser.cpp xmlparser.h \
		update.h \
		productversion.h \
		message.h \
		config.h \
		product.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o xmlparser.o xmlparser.cpp

product.o: product.cpp product.h \
		localfile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o product.o product.cpp

productversion.o: productversion.cpp productversion.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o productversion.o productversion.cpp

update.o: update.cpp update.h \
		productversion.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o update.o update.cpp

message.o: message.cpp message.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o message.o message.cpp

downloader.o: downloader.cpp downloader.h \
		update.h \
		productversion.h \
		localfile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o downloader.o downloader.cpp

osdetection.o: osdetection.cpp osdetection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o osdetection.o osdetection.cpp

commander.o: commander.cpp commander.h \
		update.h \
		productversion.h \
		settings.h \
		message.h \
		product.h \
		localfile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o commander.o commander.cpp

localfile.o: localfile.cpp localfile.h \
		settings.h \
		update.h \
		productversion.h \
		message.h \
		product.h \
		config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o localfile.o localfile.cpp

moc_dialog.o: moc_dialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_dialog.o moc_dialog.cpp

moc_sara_service.o: moc_sara_service.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_sara_service.o moc_sara_service.cpp

moc_xmlparser.o: moc_xmlparser.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_xmlparser.o moc_xmlparser.cpp

moc_downloader.o: moc_downloader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_downloader.o moc_downloader.cpp

moc_commander.o: moc_commander.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_commander.o moc_commander.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

