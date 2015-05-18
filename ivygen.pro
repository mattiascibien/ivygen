include(./qmake/gitversion.pri)
include(./qmake/deployqt.pri)

INCLUDEPATH += src

HEADERS += \
    src/BasicMesh.h \
    src/Camera.h \
    src/Common.h \
    src/Ivy.h \
    src/ivygenerator.h \
    src/OBJWriter.h \
    src/RenderWidget.h \
    src/SetupWidget.h \
    src/Vector2d.h \
    src/Vector3d.h \
    src/aboutdialog.h \
    src/birthwidget.h \
    src/objloader/list.h \
    src/objloader/obj_parser.h \
    src/objloader/objLoader.h \
    src/objloader/string_extra.h \
    src/ModelLoader.h \
    src/settingsdialog.h \
    src/colorpickerwidget.h

SOURCES += \
    src/BasicMesh.cpp \
    src/Camera.cpp \
    src/Common.cpp \
    src/Ivy.cpp \
    src/ivygenerator.cpp \
    src/main.cpp \
    src/OBJWriter.cpp \
    src/RenderWidget.cpp \
    src/SetupWidget.cpp \
    src/aboutdialog.cpp \
    src/birthwidget.cpp \
    src/objloader/list.cpp \
    src/objloader/obj_parser.cpp \
    src/objloader/objLoader.cpp \
    src/objloader/string_extra.cpp \
    src/ModelLoader.cpp \
    src/settingsdialog.cpp \
    src/colorpickerwidget.cpp

RC_FILE = resources/ivygen.rc

TARGET = ivygen
TEMPLATE = app
QT += core gui widgets opengl

data.path = $${OUT_PWD}/data
data.files = data/*

textures.path = $${OUT_PWD}/textures
textures.files = textures/*

CONFIG += c++11

INSTALLS += data textures

unix {
    LIBS += -lGLU
}

FORMS += \
    src/aboutdialog.ui \
    src/settingsdialog.ui \
    src/colorpickerwidget.ui

RESOURCES += \
    resources/ivygen.qrc
