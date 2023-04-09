TEMPLATE = lib
QT += core qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    HistoryModule.cpp \
    internal/HistoryEntry.cpp \
    internal/HistoryParser.cpp

HEADERS += \
    HistoryModule.h \
    internal/HistoryEntry.h \
    internal/HistoryParser.h

RESOURCES += \
        history.qrc
