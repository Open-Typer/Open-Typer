TEMPLATE = lib
QT += core qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    Updater.cpp \
    UpdaterModule.cpp

HEADERS += \
    Updater.h \
    UpdaterModule.h

RESOURCES += \
        updater.qrc
