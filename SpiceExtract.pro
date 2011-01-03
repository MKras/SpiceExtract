# -------------------------------------------------
# Project created by QtCreator 2009-11-16T10:30:27
# -------------------------------------------------
# QWT_DIR = /usr/local/src/qwt-5.2.0/qwt/
# QWT_DIR = /opt/qtcreator/qwt
QWT_DIR = /usr/local/
INCLUDEPATH += $$QWT_DIR/include/
LIBS += $$QWT_DIR/lib/libqwt.so.5.2.0
QT += xml
QT += script
TARGET = SpiceExtract
TEMPLATE = app
SOURCES += main.cpp \
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
    gui/optparamseditor.cpp
HEADERS += mainwindow.h \
    util/dutconfig.h \
    util/globals.h \
    core/spiceextr.h \
    core/core.h \
    gui/plot.h \
    gui/texteditor.h \
    util/extrthread.h \
    util/editorthread.h \
    gui/manualfit.h \
    gui/QScienceSpinBox.h \
    gui/curveselection.h \
    gui/about.h \
    gui/optparamseditor.h
FORMS += mainwindow.ui \
    gui/texteditor.ui \
    gui/manualfit.ui \
    gui/curveselection.ui\
    gui/about.ui \
    gui/optparamseditor.ui


# LIBS += -L/usr/local/lib  -lboost_regex -llapack -lgslcblas -llevmar -lnewton -lga -lnewmat -lopt -lf2c
# CONFIG += no_lflags_merge
LIBS += -L/usr/local/lib \ # core libs
# -lboost_regex \
    -llapack \
    -lgslcblas \
    -lgsl \
    #-llevmar \
    -lnewton \
    -lga \
    -lnewmat \
    -lopt \
    /usr/lib/libf2c.a
RESOURCES += gui/textedit.qrc
