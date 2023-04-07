TEMPLATE = lib
QT += core widgets quick
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    QmlFileDialog.cpp \
    QmlWidget.cpp \
    UiComponentsModule.cpp

HEADERS += \
    QmlFileDialog.h \
    QmlWidget.h \
    UiComponentsModule.h

RESOURCES += \
        uicomponents.qrc
