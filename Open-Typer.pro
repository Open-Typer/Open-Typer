QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += \
    -Isrc/include \
    -Isrc/include/core

SOURCES += \
    src/core/utils.cpp \
    src/core/configfile.cpp \
    src/core/net.cpp \
    src/levelsummary.cpp \
    src/packEditor/packeditor.cpp \
    src/packEditor/packview.cpp \
    src/paperconfigdialog.cpp \
    src/simplecolordialog.cpp \
    src/updater/updater.cpp \
    src/main.cpp \
    src/opentyper.cpp \
    src/updater/updaterdialog.cpp

HEADERS += \
    src/include/core/utils.h \
    src/include/core/configfile.h \
    src/include/core/net.h \
    src/include/levelsummary.h \
    src/include/packEditor/packeditor.h \
    src/include/packEditor/packview.h \
    src/include/paperconfigdialog.h \
    src/include/simplecolordialog.h \
    src/include/updater/updater.h \
    src/include/updater/updaterdialog.h \
    src/include/version.h \
    src/include/opentyper.h

FORMS += \
    ui/levelsummary.ui \
    ui/opentyper.ui \
    ui/packeditor.ui \
    ui/packview.ui \
    ui/paperconfigdialog.ui \
    ui/simplecolordialog.ui \
    ui/updaterdialog.ui

TRANSLATIONS += \
    lang/Open-Typer_sk_SK.ts

RESOURCES += \
    res.qrc

RESOURCES += \
    dark-theme/dark-style.qrc \
    light-theme/light-style.qrc

win32:RC_ICONS += res/images/icon.ico

QT += network

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
