TEMPLATE = lib
QT += core widgets quick
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    UiComponentsModule.cpp \
    internal/QmlFileDialog.cpp \
    internal/QmlWidget.cpp \
    internal/QuickWindow.cpp \
    view/DialogView.cpp

HEADERS += \
    UiComponentsModule.h \
    internal/QmlFileDialog.h \
    internal/QmlWidget.h \
    internal/QuickWindow.h \
    view/DialogView.h

wasm {
    RESOURCES += qml/OpenTyper/UiComponents/internal/platform/wasm/wasm.qrc
} else {
    RESOURCES += qml/OpenTyper/UiComponents/internal/platform/desktop/desktop.qrc
}

RESOURCES += \
        uicomponents.qrc
