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
    internal/UiEngine.cpp \
    menubar/AbstractMenuBar.cpp \
    menubar/AppMenuItem.cpp \
    menubar/AppMenuModel.cpp

HEADERS += \
    IUiEngine.h \
    QmlUtils.h \
    ThemeEngine.h \
    UiModule.h \
    internal/UiEngine.h \
    menubar/AbstractMenuBar.h \
    menubar/AppMenuItem.h \
    menubar/AppMenuModel.h
