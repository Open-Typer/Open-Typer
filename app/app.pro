TARGET = open-typer
DESTDIR = $$_PRO_FILE_PWD_/..

QT += core gui quick quickcontrols2 charts network
CONFIG += qtquickcompiler
QTPLUGIN += qsvg
!wasm {
    QT += printsupport
    LIBS += -lssl -lcrypto
}
macx {
    QT += svg
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    src \
    ../libcore/src

LIBS += -L$$_PRO_FILE_PWD_/.. -lopentyper-core

!wasm {
    SOURCES += \
        src/AddonItemModel.cpp \
	src/AddonListModel.cpp \
	src/AddonManager.cpp \
	src/AddonModel.cpp
}

SOURCES += \
    src/ExportProvider.cpp \
    src/ExportTable.cpp \
    src/ExportTableModel.cpp \
    src/QmlWidget.cpp \
    src/packEditor/PackEditor.cpp \
    src/packEditor/PackSelector.cpp \
    src/updater/Updater.cpp \
    src/main.cpp

!wasm {
    HEADERS += \
        src/AddonItemModel.h \
	src/AddonListModel.h \
	src/AddonManager.h \
	src/AddonModel.h
}

HEADERS += \
    src/ExportProvider.h \
    src/ExportTable.h \
    src/ExportTableModel.h \
    src/QmlWidget.h \
    src/packEditor/PackEditor.h \
    src/packEditor/PackSelector.h \
    src/updater/Updater.h

FORMS += \
    ui/packEditor/PackEditor.ui \
    ui/packEditor/PackSelector.ui

TRANSLATIONS += \
    translations/Open-Typer_sk_SK.ts \
    translations/Open-Typer_de_DE.ts \
    translations/Open-Typer_ru_RU.ts

RESOURCES += \
    res/icons/icons.qrc \
    res/packs/packs.qrc \
    res/splash/splash.qrc \
    translations/translations.qrc \
    ui/qml/qml.qrc

RESOURCES += \
    dark-theme/dark-style.qrc \
    light-theme/light-style.qrc

win32:RC_ICONS += res/icons/icon.ico
macx:ICON = res/macos-release/open-typer.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
