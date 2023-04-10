TEMPLATE = lib
QT += core qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    UpdaterModule.cpp

HEADERS += \
    IUpdater.h \
    UpdaterModule.h

win32 {
    SOURCES += platform/windows/WindowsUpdater.cpp
    HEADERS += platform/windows/WindowsUpdater.h
} else {
    SOURCES += platform/stub/StubUpdater.cpp
    HEADERS += platform/stub/StubUpdater.h
}

RESOURCES += \
        updater.qrc
