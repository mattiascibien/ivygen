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
    Vector3d.h

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
    SetupWidget.cpp

RC_FILE = ivygen.rc

DEFINES += QT_NO_OPENGL_ES_2

TARGET = ivygen
TEMPLATE = app
QT += core gui widgets opengl
