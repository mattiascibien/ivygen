#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T12:14:27
#
#-------------------------------------------------

TARGET = core
TEMPLATE = lib

CONFIG += c++11

QT += core gui opengl

INCLUDEPATH += .

DEFINES += CORE_LIBRARY _CRT_SECURE_NO_WARNINGS

SOURCES += \
    BasicMesh.cpp \
    Camera.cpp \
    ModelLoader.cpp \
    OBJWriter.cpp \
    importerplugin.cpp \
    objloader/list.cpp \
    objloader/obj_parser.cpp \
    objloader/objLoader.cpp \
    objloader/string_extra.cpp

HEADERS += \
    core_global.h \
    BasicMesh.h \
    Camera.h \
    ModelLoader.h \
    OBJWriter.h \
    Vector2d.h \
    Vector3d.h \
    importerplugin.h \
    objloader/list.h \
    objloader/obj_parser.h \
    objloader/objLoader.h \
    objloader/string_extra.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
