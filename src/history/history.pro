TEMPLATE = lib
QT += core qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    HistoryEntry.cpp \
    HistoryModule.cpp \
    HistoryParser.cpp

HEADERS += \
    HistoryEntry.h \
    HistoryModule.h \
    HistoryParser.h

RESOURCES += \
        history.qrc
