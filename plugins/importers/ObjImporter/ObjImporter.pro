#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T11:44:14
#
#-------------------------------------------------

QT       += core gui

TARGET = ObjImporter
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/generic

SOURCES += objimporter.cpp

HEADERS += objimporter.h
DISTFILES += ObjImporter.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
