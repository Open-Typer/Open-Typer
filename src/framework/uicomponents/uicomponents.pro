TEMPLATE = lib
QT += core widgets quick
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    UiComponentsModule.cpp \
    internal/QmlFileDialog.cpp \
    internal/QmlWidget.cpp

HEADERS += \
    UiComponentsModule.h \
    internal/QmlFileDialog.h \
    internal/QmlWidget.h

RESOURCES += \
        uicomponents.qrc
