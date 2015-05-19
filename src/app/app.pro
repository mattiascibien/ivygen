INCLUDEPATH += .

HEADERS += \
    ivygeneratorwindow.h \
    settingsdialog.h \
    aboutdialog.h \
    birthwidget.h \
    colorpickerwidget.h \
    SetupWidget.h \
    BirthWidget.h \
    RenderWidget.h \
    Ivy.h \
    Common.h \

SOURCES += \
    ivygeneratorwindow.cpp \
    aboutdialog.cpp \
    birthwidget.cpp \
    colorpickerwidget.cpp \
    settingsdialog.cpp \
    SetupWidget.cpp \
    BirthWidget.cpp \
    RenderWidget.cpp \
    Ivy.cpp \
    Common.cpp \
    main.cpp

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
    aboutdialog.ui \
    settingsdialog.ui \
    colorpickerwidget.ui

RESOURCES += \
    resources/ivygen.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core


include(../../qmake/gitversion.pri)
include(../../qmake/deployqt.pri)
