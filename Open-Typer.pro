QT       += core gui charts

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

INCLUDEPATH += \
    src/include

SOURCES += \
    src/core/utils.cpp \
    src/core/configfile.cpp \
    src/core/net.cpp \
    src/core/packs.cpp \
    src/core/language.cpp \
    src/levelsummary.cpp \
    src/options/connection.cpp \
    src/options/studentoptions.cpp \
    src/statsdialog.cpp \
    src/timedialog.cpp \
    src/options/behavior.cpp \
    src/options/customization.cpp \
    src/options/keyboard.cpp \
    src/options/optionswindow.cpp \
    src/packEditor/packeditor.cpp \
    src/packEditor/packview.cpp \
    src/packEditor/packselector.cpp \
    src/paperconfigdialog.cpp \
    src/simplecolordialog.cpp \
    src/updater/updater.cpp \
    src/main.cpp \
    src/opentyper.cpp \
    src/updater/updaterdialog.cpp \
    src/widgets/inputlabel.cpp \
    src/widgets/languagelist.cpp \
    src/widgets/keyboardwidget.cpp

HEADERS += \
    src/include/core/utils.h \
    src/include/core/configfile.h \
    src/include/core/net.h \
    src/include/core/packs.h \
    src/include/core/language.h \
    src/include/levelsummary.h \
    src/include/options/connection.h \
    src/include/options/studentoptions.h \
    src/include/statsdialog.h \
    src/include/timedialog.h \
    src/include/options/behavior.h \
    src/include/options/customization.h \
    src/include/options/keyboard.h \
    src/include/options/optionswindow.h \
    src/include/packEditor/packeditor.h \
    src/include/packEditor/packview.h \
    src/include/packEditor/packselector.h \
    src/include/paperconfigdialog.h \
    src/include/simplecolordialog.h \
    src/include/updater/updater.h \
    src/include/updater/updaterdialog.h \
    src/include/version.h \
    src/include/opentyper.h \
    src/include/widgets/inputlabel.h \
    src/include/widgets/languagelist.h \
    src/include/widgets/keyboardwidget.h

FORMS += \
    ui/options/behavior.ui \
    ui/options/connection.ui \
    ui/options/customization.ui \
    ui/options/keyboard.ui \
    ui/levelsummary.ui \
    ui/options/studentoptions.ui \
    ui/statsdialog.ui \
    ui/timedialog.ui \
    ui/opentyper.ui \
    ui/options/optionswindow.ui \
    ui/packEditor/packeditor.ui \
    ui/packEditor/packselector.ui \
    ui/packEditor/packview.ui \
    ui/paperconfigdialog.ui \
    ui/simplecolordialog.ui \
    ui/updater/updaterdialog.ui

TRANSLATIONS += \
    lang/Open-Typer_sk_SK.ts \
    lang/Open-Typer_de_DE.ts

RESOURCES += \
    res.qrc \
    certs/certs.qrc \
    res/keyboard-layouts/layouts.qrc

RESOURCES += \
    dark-theme/dark-style.qrc \
    light-theme/light-style.qrc

win32:RC_ICONS += res/images/icon.ico

QT += network

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
