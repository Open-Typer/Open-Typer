TEMPLATE = lib
QT += core quick
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    KeyboardModule.cpp \
    internal/Key.cpp \
    internal/KeyboardLayout.cpp \
    internal/KeyboardUtils.cpp \
    internal/QmlKeyboardHandler.cpp

HEADERS += \
    KeyboardModule.h \
    internal/Key.h \
    internal/KeyboardLayout.h \
    internal/KeyboardUtils.h \
    internal/QmlKeyboardHandler.h

# Third party
include(../../../3rdparty/xkeyboard-config/xkeyboard-config.pri)
include(../../../3rdparty/libxkbcommon/libxkbcommon.pri)

RESOURCES += keyboard.qrc
