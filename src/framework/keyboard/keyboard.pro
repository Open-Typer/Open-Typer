TEMPLATE = lib
QT += core quick
CONFIG += staticlib

SOURCES += \
    Key.cpp \
    KeyboardLayout.cpp \
    KeyboardUtils.cpp \
    QmlKeyboardHandler.cpp

HEADERS += \
    Key.h \
    KeyboardLayout.h \
    KeyboardUtils.h \
    QmlKeyboardHandler.h

# Third party
include(../../../3rdparty/xkeyboard-config/xkeyboard-config.pri)
include(../../../3rdparty/libxkbcommon/libxkbcommon.pri)
