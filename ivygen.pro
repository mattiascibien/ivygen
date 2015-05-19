include(./qmake/gitversion.pri)
include(./qmake/deployqt.pri)

INCLUDEPATH += src

HEADERS += \
    src/core/BasicMesh.h \
    src/core/Camera.h \
    src/core/Common.h \
    src/core/Ivy.h \
    src/core/ModelLoader.h \
    src/core/OBJWriter.h \
    src/core/math/Vector2d.h \
    src/core/math/Vector3d.h \
    src/objloader/list.h \
    src/objloader/obj_parser.h \
    src/objloader/objLoader.h \
    src/objloader/string_extra.h \
    src/gui/ivygeneratorwindow.h \
    src/gui/settingsdialog.h \
    src/gui/RenderWidget.h \
    src/gui/SetupWidget.h \
    src/gui/aboutdialog.h \
    src/gui/birthwidget.h \
    src/gui/colorpickerwidget.h \
    src/extensibility/importerplugin.h

SOURCES += \
    src/core/BasicMesh.cpp \
    src/core/Camera.cpp \
    src/core/Common.cpp \
    src/core/Ivy.cpp \
    src/core/ModelLoader.cpp \
    src/core/OBJWriter.cpp \
    src/objloader/list.cpp \
    src/objloader/obj_parser.cpp \
    src/objloader/objLoader.cpp \
    src/objloader/string_extra.cpp \
    src/gui/ivygeneratorwindow.cpp \
    src/gui/RenderWidget.cpp \
    src/gui/SetupWidget.cpp \
    src/gui/aboutdialog.cpp \
    src/gui/birthwidget.cpp \
    src/gui/colorpickerwidget.cpp \
    src/gui/settingsdialog.cpp \
    src/main.cpp \
    src/extensibility/importerplugin.cpp

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
    src/gui/aboutdialog.ui \
    src/gui/settingsdialog.ui \
    src/gui/colorpickerwidget.ui

RESOURCES += \
    resources/ivygen.qrc
