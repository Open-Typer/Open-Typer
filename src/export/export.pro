TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

!wasm {
    QT += printsupport
}

SOURCES += \
    ExportModule.cpp \
    internal/ExportProvider.cpp \
    internal/ExportTable.cpp \
    internal/ExportTableModel.cpp

HEADERS += \
    ExportModule.h \
    internal/ExportProvider.h \
    internal/ExportTable.h \
    internal/ExportTableModel.h
