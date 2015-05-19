include(./qmake/gitversion.pri)
include(./qmake/deployqt.pri)

INCLUDEPATH += src

HEADERS += \
    src/gui/ivygeneratorwindow.h \
    src/gui/settingsdialog.h \
    src/gui/aboutdialog.h \
    src/gui/birthwidget.h \
    src/gui/colorpickerwidget.h

SOURCES += \
    src/gui/ivygeneratorwindow.cpp \
    src/gui/aboutdialog.cpp \
    src/gui/birthwidget.cpp \
    src/gui/colorpickerwidget.cpp \
    src/gui/settingsdialog.cpp \
    src/main.cpp

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
