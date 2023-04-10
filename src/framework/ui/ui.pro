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
    UiModule.cpp \
    internal/QmlUtils.cpp \
    internal/ThemeEngine.cpp \
    internal/UiEngine.cpp \
    menubar/AbstractMenuBar.cpp \
    menubar/AppMenuItem.cpp \
    menubar/AppMenuModel.cpp

HEADERS += \
    IThemeEngine.h \
    IUiEngine.h \
    UiModule.h \
    internal/QmlUtils.h \
    internal/ThemeEngine.h \
    internal/UiEngine.h \
    menubar/AbstractMenuBar.h \
    menubar/AppMenuItem.h \
    menubar/AppMenuModel.h
