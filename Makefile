#############################################################################
# Makefile for building: SpiceExtract
# Generated by qmake (2.01a) (Qt 4.6.3) on: ?? ???. 5 13:49:17 2010
# Project:  SpiceExtract.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile SpiceExtract.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_SCRIPT_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -fmessage-length=0 -O2 -Wall -D_FORTIFY_SOURCE=2 -fstack-protector -funwind-tables -fasynchronous-unwind-tables -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -fmessage-length=0 -O2 -Wall -D_FORTIFY_SOURCE=2 -fstack-protector -funwind-tables -fasynchronous-unwind-tables -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/default -I. -I/usr/include/QtCore -I/usr/include/QtGui -I/usr/include/QtXml -I/usr/include/QtScript -I/usr/include -I/usr/local/include -I. -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib64 /usr/local//lib/libqwt.so.5.2.0 -L/usr/local/lib -llapack -lgslcblas -lgsl -llevmar -lnewton -lga -lnewmat -lopt /usr/lib/libf2c.a -lQtScript -L/usr/lib64 -lQtXml -lQtGui -L/usr/X11R6/lib64 -lQtCore -lpthread 
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
		mainwindow.cpp \
		util/dutconfig.cpp \
		core/spiceextr.cpp \
		core/core.cpp \
		gui/plot.cpp \
		gui/texteditor.cpp \
		util/extrthread.cpp \
		util/editorthread.cpp \
		gui/manualfit.cpp \
		gui/QScienceSpinBox.cpp \
		gui/curveselection.cpp \
		gui/about.cpp \
		gui/optparamseditor.cpp moc_mainwindow.cpp \
		moc_spiceextr.cpp \
		moc_plot.cpp \
		moc_texteditor.cpp \
		moc_extrthread.cpp \
		moc_editorthread.cpp \
		moc_manualfit.cpp \
		moc_QScienceSpinBox.cpp \
		moc_curveselection.cpp \
		moc_about.cpp \
		moc_optparamseditor.cpp \
		qrc_textedit.cpp
OBJECTS       = main.o \
		mainwindow.o \
		dutconfig.o \
		spiceextr.o \
		core.o \
		plot.o \
		texteditor.o \
		extrthread.o \
		editorthread.o \
		manualfit.o \
		QScienceSpinBox.o \
		curveselection.o \
		about.o \
		optparamseditor.o \
		moc_mainwindow.o \
		moc_spiceextr.o \
		moc_plot.o \
		moc_texteditor.o \
		moc_extrthread.o \
		moc_editorthread.o \
		moc_manualfit.o \
		moc_QScienceSpinBox.o \
		moc_curveselection.o \
		moc_about.o \
		moc_optparamseditor.o \
		qrc_textedit.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
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
		SpiceExtract.pro
QMAKE_TARGET  = SpiceExtract
DESTDIR       = 
TARGET        = SpiceExtract

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

$(TARGET): ui_mainwindow.h ui_texteditor.h ui_manualfit.h ui_curveselection.h ui_about.h ui_optparamseditor.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: SpiceExtract.pro  /usr/share/qt4/mkspecs/default/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
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
		/usr/lib64/libQtScript.prl \
		/usr/lib64/libQtCore.prl \
		/usr/lib64/libQtXml.prl \
		/usr/lib64/libQtGui.prl
	$(QMAKE) -unix -o Makefile SpiceExtract.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
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
/usr/lib64/libQtScript.prl:
/usr/lib64/libQtCore.prl:
/usr/lib64/libQtXml.prl:
/usr/lib64/libQtGui.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile SpiceExtract.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/SpiceExtract1.0.0 || $(MKDIR) .tmp/SpiceExtract1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/SpiceExtract1.0.0/ && $(COPY_FILE) --parents mainwindow.h util/dutconfig.h util/globals.h core/spiceextr.h core/core.h gui/plot.h gui/texteditor.h util/extrthread.h util/editorthread.h gui/manualfit.h gui/QScienceSpinBox.h gui/curveselection.h gui/about.h gui/optparamseditor.h .tmp/SpiceExtract1.0.0/ && $(COPY_FILE) --parents gui/textedit.qrc .tmp/SpiceExtract1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp util/dutconfig.cpp core/spiceextr.cpp core/core.cpp gui/plot.cpp gui/texteditor.cpp util/extrthread.cpp util/editorthread.cpp gui/manualfit.cpp gui/QScienceSpinBox.cpp gui/curveselection.cpp gui/about.cpp gui/optparamseditor.cpp .tmp/SpiceExtract1.0.0/ && $(COPY_FILE) --parents mainwindow.ui gui/texteditor.ui gui/manualfit.ui gui/curveselection.ui gui/about.ui gui/optparamseditor.ui .tmp/SpiceExtract1.0.0/ && (cd `dirname .tmp/SpiceExtract1.0.0` && $(TAR) SpiceExtract1.0.0.tar SpiceExtract1.0.0 && $(COMPRESS) SpiceExtract1.0.0.tar) && $(MOVE) `dirname .tmp/SpiceExtract1.0.0`/SpiceExtract1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/SpiceExtract1.0.0


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

compiler_moc_header_make_all: moc_mainwindow.cpp moc_spiceextr.cpp moc_plot.cpp moc_texteditor.cpp moc_extrthread.cpp moc_editorthread.cpp moc_manualfit.cpp moc_QScienceSpinBox.cpp moc_curveselection.cpp moc_about.cpp moc_optparamseditor.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_spiceextr.cpp moc_plot.cpp moc_texteditor.cpp moc_extrthread.cpp moc_editorthread.cpp moc_manualfit.cpp moc_QScienceSpinBox.cpp moc_curveselection.cpp moc_about.cpp moc_optparamseditor.cpp
moc_mainwindow.cpp: util/dutconfig.h \
		util/globals.h \
		util/extrthread.h \
		core/core.h \
		core/spiceextr.h \
		util/editorthread.h \
		gui/texteditor.h \
		gui/plot.h \
		gui/manualfit.h \
		gui/QScienceSpinBox.h \
		gui/curveselection.h \
		gui/about.h \
		gui/optparamseditor.h \
		mainwindow.h
	/usr/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_spiceextr.cpp: util/globals.h \
		core/spiceextr.h
	/usr/bin/moc $(DEFINES) $(INCPATH) core/spiceextr.h -o moc_spiceextr.cpp

moc_plot.cpp: gui/plot.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gui/plot.h -o moc_plot.cpp

moc_texteditor.cpp: gui/texteditor.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gui/texteditor.h -o moc_texteditor.cpp

moc_extrthread.cpp: util/globals.h \
		core/core.h \
		core/spiceextr.h \
		util/extrthread.h
	/usr/bin/moc $(DEFINES) $(INCPATH) util/extrthread.h -o moc_extrthread.cpp

moc_editorthread.cpp: gui/texteditor.h \
		util/extrthread.h \
		util/globals.h \
		core/core.h \
		core/spiceextr.h \
		util/editorthread.h
	/usr/bin/moc $(DEFINES) $(INCPATH) util/editorthread.h -o moc_editorthread.cpp

moc_manualfit.cpp: util/globals.h \
		gui/QScienceSpinBox.h \
		util/extrthread.h \
		core/core.h \
		core/spiceextr.h \
		gui/manualfit.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gui/manualfit.h -o moc_manualfit.cpp

moc_QScienceSpinBox.cpp: gui/QScienceSpinBox.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gui/QScienceSpinBox.h -o moc_QScienceSpinBox.cpp

moc_curveselection.cpp: util/globals.h \
		util/extrthread.h \
		core/core.h \
		core/spiceextr.h \
		util/editorthread.h \
		gui/texteditor.h \
		gui/curveselection.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gui/curveselection.h -o moc_curveselection.cpp

moc_about.cpp: gui/about.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gui/about.h -o moc_about.cpp

moc_optparamseditor.cpp: util/globals.h \
		gui/optparamseditor.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gui/optparamseditor.h -o moc_optparamseditor.cpp

compiler_rcc_make_all: qrc_textedit.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_textedit.cpp
qrc_textedit.cpp: gui/textedit.qrc \
		gui/images/logo32.png \
		gui/images/mac/editredo.png \
		gui/images/mac/exportpdf.png \
		gui/images/mac/textright.png \
		gui/images/mac/fileprint.png \
		gui/images/mac/editcopy.png \
		gui/images/mac/textunder.png \
		gui/images/mac/editundo.png \
		gui/images/mac/filesave.png \
		gui/images/mac/editcut.png \
		gui/images/mac/fileopen.png \
		gui/images/mac/textcenter.png \
		gui/images/mac/zoomout.png \
		gui/images/mac/textitalic.png \
		gui/images/mac/filenew.png \
		gui/images/mac/textleft.png \
		gui/images/mac/textbold.png \
		gui/images/mac/editpaste.png \
		gui/images/mac/zoomin.png \
		gui/images/mac/textjustify.png \
		gui/images/win/editredo.png \
		gui/images/win/exportpdf.png \
		gui/images/win/textright.png \
		gui/images/win/fileprint.png \
		gui/images/win/editcopy.png \
		gui/images/win/textunder.png \
		gui/images/win/editundo.png \
		gui/images/win/filesave.png \
		gui/images/win/editcut.png \
		gui/images/win/fileopen.png \
		gui/images/win/textcenter.png \
		gui/images/win/zoomout.png \
		gui/images/win/textitalic.png \
		gui/images/win/filenew.png \
		gui/images/win/textleft.png \
		gui/images/win/textbold.png \
		gui/images/win/editpaste.png \
		gui/images/win/zoomin.png \
		gui/images/win/textjustify.png
	/usr/bin/rcc -name textedit gui/textedit.qrc -o qrc_textedit.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h ui_texteditor.h ui_manualfit.h ui_curveselection.h ui_about.h ui_optparamseditor.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h ui_texteditor.h ui_manualfit.h ui_curveselection.h ui_about.h ui_optparamseditor.h
ui_mainwindow.h: mainwindow.ui
	/usr/bin/uic mainwindow.ui -o ui_mainwindow.h

ui_texteditor.h: gui/texteditor.ui
	/usr/bin/uic gui/texteditor.ui -o ui_texteditor.h

ui_manualfit.h: gui/manualfit.ui
	/usr/bin/uic gui/manualfit.ui -o ui_manualfit.h

ui_curveselection.h: gui/curveselection.ui
	/usr/bin/uic gui/curveselection.ui -o ui_curveselection.h

ui_about.h: gui/about.ui
	/usr/bin/uic gui/about.ui -o ui_about.h

ui_optparamseditor.h: gui/optparamseditor.ui
	/usr/bin/uic gui/optparamseditor.ui -o ui_optparamseditor.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		util/dutconfig.h \
		util/globals.h \
		util/extrthread.h \
		core/core.h \
		core/spiceextr.h \
		util/editorthread.h \
		gui/texteditor.h \
		gui/plot.h \
		gui/manualfit.h \
		gui/QScienceSpinBox.h \
		gui/curveselection.h \
		gui/about.h \
		gui/optparamseditor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		util/dutconfig.h \
		util/globals.h \
		util/extrthread.h \
		core/core.h \
		core/spiceextr.h \
		util/editorthread.h \
		gui/texteditor.h \
		gui/plot.h \
		gui/manualfit.h \
		gui/QScienceSpinBox.h \
		gui/curveselection.h \
		gui/about.h \
		gui/optparamseditor.h \
		ui_mainwindow.h \
		util/duttreemodel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

dutconfig.o: util/dutconfig.cpp util/dutconfig.h \
		util/globals.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dutconfig.o util/dutconfig.cpp

spiceextr.o: core/spiceextr.cpp core/spiceextr.h \
		util/globals.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o spiceextr.o core/spiceextr.cpp

core.o: core/core.cpp core/core.h \
		util/globals.h \
		core/spiceextr.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o core.o core/core.cpp

plot.o: gui/plot.cpp gui/plot.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o plot.o gui/plot.cpp

texteditor.o: gui/texteditor.cpp gui/texteditor.h \
		ui_texteditor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o texteditor.o gui/texteditor.cpp

extrthread.o: util/extrthread.cpp util/extrthread.h \
		util/globals.h \
		core/core.h \
		core/spiceextr.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o extrthread.o util/extrthread.cpp

editorthread.o: util/editorthread.cpp util/editorthread.h \
		gui/texteditor.h \
		util/extrthread.h \
		util/globals.h \
		core/core.h \
		core/spiceextr.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o editorthread.o util/editorthread.cpp

manualfit.o: gui/manualfit.cpp gui/manualfit.h \
		util/globals.h \
		gui/QScienceSpinBox.h \
		util/extrthread.h \
		core/core.h \
		core/spiceextr.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o manualfit.o gui/manualfit.cpp

QScienceSpinBox.o: gui/QScienceSpinBox.cpp gui/QScienceSpinBox.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o QScienceSpinBox.o gui/QScienceSpinBox.cpp

curveselection.o: gui/curveselection.cpp gui/curveselection.h \
		util/globals.h \
		util/extrthread.h \
		core/core.h \
		core/spiceextr.h \
		util/editorthread.h \
		gui/texteditor.h \
		ui_curveselection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o curveselection.o gui/curveselection.cpp

about.o: gui/about.cpp gui/about.h \
		ui_about.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o about.o gui/about.cpp

optparamseditor.o: gui/optparamseditor.cpp gui/optparamseditor.h \
		util/globals.h \
		ui_optparamseditor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o optparamseditor.o gui/optparamseditor.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_spiceextr.o: moc_spiceextr.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_spiceextr.o moc_spiceextr.cpp

moc_plot.o: moc_plot.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_plot.o moc_plot.cpp

moc_texteditor.o: moc_texteditor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_texteditor.o moc_texteditor.cpp

moc_extrthread.o: moc_extrthread.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_extrthread.o moc_extrthread.cpp

moc_editorthread.o: moc_editorthread.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_editorthread.o moc_editorthread.cpp

moc_manualfit.o: moc_manualfit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_manualfit.o moc_manualfit.cpp

moc_QScienceSpinBox.o: moc_QScienceSpinBox.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_QScienceSpinBox.o moc_QScienceSpinBox.cpp

moc_curveselection.o: moc_curveselection.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_curveselection.o moc_curveselection.cpp

moc_about.o: moc_about.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_about.o moc_about.cpp

moc_optparamseditor.o: moc_optparamseditor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_optparamseditor.o moc_optparamseditor.cpp

qrc_textedit.o: qrc_textedit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_textedit.o qrc_textedit.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

