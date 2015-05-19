#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T14:12:04
#
#-------------------------------------------------

QT       += core gui

TARGET = objimporter
TEMPLATE = lib
CONFIG += plugin

#DESTDIR = $$[QT_INSTALL_PLUGINS]/importers

SOURCES += objimporter.cpp

HEADERS += objimporter.h
DISTFILES += objimporter.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../../core/ -lcore

INCLUDEPATH += $$PWD/../../../core
DEPENDPATH += $$PWD/../../../core
