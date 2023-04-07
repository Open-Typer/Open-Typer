TEMPLATE = lib
QT += core quick
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    Key.cpp \
    KeyboardLayout.cpp \
    KeyboardModule.cpp \
    KeyboardUtils.cpp \
    QmlKeyboardHandler.cpp

HEADERS += \
    Key.h \
    KeyboardLayout.h \
    KeyboardModule.h \
    KeyboardUtils.h \
    QmlKeyboardHandler.h

# Third party
include(../../../3rdparty/xkeyboard-config/xkeyboard-config.pri)
include(../../../3rdparty/libxkbcommon/libxkbcommon.pri)

RESOURCES += keyboard.qrc
