HEADERS += \
    src/BasicMesh.h \
    src/Camera.h \
    src/Common.h \
    src/Ivy.h \
    src/ivygenerator.h \
    src/OBJLoader.h \
    src/OBJWriter.h \
    src/RenderWidget.h \
    src/SetupWidget.h \
    src/Vector2d.h \
    src/Vector3d.h \
    src/aboutdialog.h \
    src/birthwidget.h

SOURCES += \
    src/BasicMesh.cpp \
    src/Camera.cpp \
    src/Common.cpp \
    src/Ivy.cpp \
    src/ivygenerator.cpp \
    src/main.cpp \
    src/OBJLoader.cpp \
    src/OBJWriter.cpp \
    src/RenderWidget.cpp \
    src/SetupWidget.cpp \
    src/aboutdialog.cpp \
    src/birthwidget.cpp

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
    src/aboutdialog.ui

RESOURCES += \
    resources/ivygen.qrc
