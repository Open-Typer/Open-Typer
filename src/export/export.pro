TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

!wasm {
    QT += printsupport
}

SOURCES += \
    ExportProvider.cpp \
    ExportTable.cpp \
    ExportTableModel.cpp

HEADERS += \
    ExportProvider.h \
    ExportTable.h \
    ExportTableModel.h
