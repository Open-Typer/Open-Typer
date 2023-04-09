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
    ThemeEngine.cpp \
    UiModule.cpp \
    internal/QmlUtils.cpp \
    internal/UiEngine.cpp \
    menubar/AbstractMenuBar.cpp \
    menubar/AppMenuItem.cpp \
    menubar/AppMenuModel.cpp

HEADERS += \
    IUiEngine.h \
    ThemeEngine.h \
    UiModule.h \
    internal/QmlUtils.h \
    internal/UiEngine.h \
    menubar/AbstractMenuBar.h \
    menubar/AppMenuItem.h \
    menubar/AppMenuModel.h
