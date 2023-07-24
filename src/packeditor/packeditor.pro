TEMPLATE = lib
QT += core qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    PackEditorModule.cpp \
    internal/PackEditorModel.cpp

HEADERS += \
    PackEditorModule.h \
    internal/PackEditorModel.h

RESOURCES += \
    packeditor.qrc
