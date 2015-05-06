HEADERS += \
    BasicMesh.h \
    Camera.h \
    Common.h \
    Ivy.h \
    ivygenerator.h \
    OBJLoader.h \
    OBJWriter.h \
    RenderWidget.h \
    SetupWidget.h \
    Vector2d.h \
    Vector3d.h \
    aboutdialog.h

SOURCES += \
    BasicMesh.cpp \
    Camera.cpp \
    Common.cpp \
    Ivy.cpp \
    ivygenerator.cpp \
    main.cpp \
    OBJLoader.cpp \
    OBJWriter.cpp \
    RenderWidget.cpp \
    SetupWidget.cpp \
    aboutdialog.cpp

RC_FILE = ivygen.rc

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
    aboutdialog.ui
