#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T12:14:27
#
#-------------------------------------------------

TARGET = core
TEMPLATE = lib

QT += core gui opengl

INCLUDEPATH += src

DEFINES += CORE_LIBRARY _CRT_SECURE_NO_WARNINGS

SOURCES += \
    src/core/BasicMesh.cpp \
    src/core/Camera.cpp \
    src/core/Common.cpp \
    src/core/Ivy.cpp \
    src/core/ModelLoader.cpp \
    src/core/OBJWriter.cpp \
    src/core/importerplugin.cpp \
    src/objloader/list.cpp \
    src/objloader/obj_parser.cpp \
    src/objloader/objLoader.cpp \
    src/objloader/string_extra.cpp \
    src/gui/SetupWidget.cpp \
    src/gui/BirthWidget.cpp

HEADERS += \
    src/core/BasicMesh.h \
    src/core/Camera.h \
    src/core/Common.h \
    src/core/Ivy.h \
    src/core/ModelLoader.h \
    src/core/OBJWriter.h \
    src/core/math/Vector2d.h \
    src/core/math/Vector3d.h \
    src/core/importerplugin.h \
    src/objloader/list.h \
    src/objloader/obj_parser.h \
    src/objloader/objLoader.h \
    src/objloader/string_extra.h \
    src/gui/SetupWidget.h \
    src/gui/BirthWidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
