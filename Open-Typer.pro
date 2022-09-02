TARGET = open-typer

QT += core gui charts network websockets
QTPLUGIN += qsvg
!wasm {
    QT += printsupport sql
    LIBS += -lssl -lcrypto
}

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

# Version and revision
win32 {
    GIT_LOCATION = $$system(where git)
}
!win32 {
    GIT_LOCATION = $$system(which git)
}
isEmpty(GIT_LOCATION) | !exists(.git) {
    exists(version) : exists(revision) {
        DEFINES += BUILD_VERSION=\\\"$$cat(version)\\\"
        DEFINES += BUILD_REVISION=\\\"$$cat(revision)\\\"
    }
} else {
    DEFINES += BUILD_VERSION=\\\"$$system(git describe --tags --abbrev=0)\\\"
    DEFINES += BUILD_REVISION=\\\"$$system(git rev-parse --short HEAD)\\\"
}

INCLUDEPATH += \
    src/include

win32 {
    INCLUDEPATH += C:/OpenSSL/include
}

SOURCES += \
    src/core/utils.cpp \
    src/core/configfile.cpp \
    src/core/database.cpp
!wasm {
    SOURCES += src/core/server.cpp
}
SOURCES += \
    src/core/net.cpp \
    src/core/packs.cpp \
    src/core/language.cpp \
    src/core/theme.cpp \
    src/initialsetup.cpp \
    src/levelsummary.cpp \
    src/options/connection.cpp \
    src/options/studentoptions.cpp \
    src/statsdialog.cpp \
    src/timedialog.cpp \
    src/testwaitdialog.cpp \
    src/options/behavior.cpp \
    src/options/appearance.cpp \
    src/options/keyboard.cpp \
    src/options/optionswindow.cpp \
    src/packEditor/packeditor.cpp \
    src/packEditor/packselector.cpp \
    src/updater/updater.cpp \
    src/main.cpp \
    src/MainWindow.cpp \
    src/exportdialog.cpp \
    src/server/usermanager.cpp \
    src/server/useredit.cpp \
    src/server/adminselector.cpp \
    src/server/serversetup.cpp \
    src/server/classedit.cpp \
    src/server/studentedit.cpp \
    src/server/deviceedit.cpp \
    src/server/loadexercisedialog.cpp \
    src/server/exerciseprogressdialog.cpp \
    src/server/widgets/servermanager.cpp \
    src/server/widgets/classcontrols.cpp \
    src/server/widgets/studentdetails.cpp \
    src/updater/updaterdialog.cpp \
    src/widgets/inputlabel.cpp \
    src/widgets/textview.cpp \
    src/widgets/languagelist.cpp \
    src/widgets/keyboardwidget.cpp

HEADERS += \
    src/include/core/utils.h \
    src/include/core/configfile.h \
    src/include/core/database.h
!wasm {
    HEADERS += src/include/core/server.h
}
HEADERS += \
    src/include/core/net.h \
    src/include/core/packs.h \
    src/include/core/language.h \
    src/include/core/theme.h \
    src/include/initialsetup.h \
    src/include/levelsummary.h \
    src/include/options/connection.h \
    src/include/options/studentoptions.h \
    src/include/statsdialog.h \
    src/include/timedialog.h \
    src/include/testwaitdialog.h \
    src/include/options/behavior.h \
    src/include/options/appearance.h \
    src/include/options/keyboard.h \
    src/include/options/optionswindow.h \
    src/include/packEditor/packeditor.h \
    src/include/packEditor/packselector.h \
    src/include/updater/updater.h \
    src/include/updater/updaterdialog.h \
    src/include/MainWindow.h \
    src/include/exportdialog.h \
    src/include/server/usermanager.h \
    src/include/server/useredit.h \
    src/include/server/adminselector.h \
    src/include/server/serversetup.h \
    src/include/server/classedit.h \
    src/include/server/studentedit.h \
    src/include/server/deviceedit.h \
    src/include/server/loadexercisedialog.h \
    src/include/server/exerciseprogressdialog.h \
    src/include/server/widgets/servermanager.h \
    src/include/server/widgets/classcontrols.h \
    src/include/server/widgets/studentdetails.h \
    src/include/widgets/inputlabel.h \
    src/include/widgets/textview.h \
    src/include/widgets/languagelist.h \
    src/include/widgets/keyboardwidget.h

FORMS += \
    ui/initialsetup.ui \
    ui/options/behavior.ui \
    ui/options/connection.ui \
    ui/options/appearance.ui \
    ui/options/keyboard.ui \
    ui/levelsummary.ui \
    ui/options/studentoptions.ui \
    ui/statsdialog.ui \
    ui/timedialog.ui \
    ui/testwaitdialog.ui \
    ui/MainWindow.ui \
    ui/exportdialog.ui \
    ui/server/usermanager.ui \
    ui/server/useredit.ui \
    ui/server/adminselector.ui \
    ui/server/serversetup.ui \
    ui/server/classedit.ui \
    ui/server/studentedit.ui \
    ui/server/deviceedit.ui \
    ui/server/loadexercisedialog.ui \
    ui/server/exerciseprogressdialog.ui \
    ui/server/widgets/servermanager.ui \
    ui/server/widgets/classcontrols.ui \
    ui/server/widgets/studentdetails.ui \
    ui/options/optionswindow.ui \
    ui/packEditor/packeditor.ui \
    ui/packEditor/packselector.ui \
    ui/updater/updaterdialog.ui

TRANSLATIONS += \
    lang/Open-Typer_sk_SK.ts \
    lang/Open-Typer_de_DE.ts

RESOURCES += \
    res.qrc \
    res/images/class-icons/class-icons.qrc \
    res/keyboard-layouts/layouts.qrc

RESOURCES += \
    dark-theme/dark-style.qrc \
    light-theme/light-style.qrc

win32:RC_ICONS += res/images/icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
