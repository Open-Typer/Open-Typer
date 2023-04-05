TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

!wasm {
    QT += printsupport
}

SOURCES += \
    ExportModule.cpp \
    ExportProvider.cpp \
    ExportTable.cpp \
    ExportTableModel.cpp

HEADERS += \
    ExportModule.h \
    ExportProvider.h \
    ExportTable.h \
    ExportTableModel.h
