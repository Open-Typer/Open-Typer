TEMPLATE = lib
QT += core widgets quick
CONFIG += staticlib

linux {
    QT += dbus
}

!wasm {
    QT += printsupport
}

SOURCES += \
    QmlUtils.cpp \
    ThemeEngine.cpp \
    UiModule.cpp \
    menubar/AbstractMenuBar.cpp \
    menubar/AppMenuItem.cpp \
    menubar/AppMenuModel.cpp

HEADERS += \
    QmlUtils.h \
    ThemeEngine.h \
    UiModule.h \
    menubar/AbstractMenuBar.h \
    menubar/AppMenuItem.h \
    menubar/AppMenuModel.h
