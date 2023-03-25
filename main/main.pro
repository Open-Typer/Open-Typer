TEMPLATE = app
TARGET = open-typer
DESTDIR = $$_PRO_FILE_PWD_/..

QT += core gui quick quickcontrols2
QTPLUGIN += qsvg

!wasm {
    QT += printsupport
    LIBS += -lssl -lcrypto
}
macx {
    QT += svg
}

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

LIBS += -L$$_PRO_FILE_PWD_/.. -lopentyper-core
INCLUDEPATH += ../src

SOURCES += ../src/main.cpp

RESOURCES += \
    ../res/icons/icons.qrc \
    ../res/packs/packs.qrc \
    ../res/splash/splash.qrc \
    ../src/ui/qml/qml.qrc

RESOURCES += \
    ../dark-theme/dark-style.qrc \
    ../light-theme/light-style.qrc

win32:RC_ICONS += ../res/icons/icon.ico
macx:ICON = ../res/macos-release/open-typer.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
