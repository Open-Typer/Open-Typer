TEMPLATE = app
TARGET = open-typer

QT += core gui widgets quick quickcontrols2
QTPLUGIN += qsvg

linux {
    QT += dbus
}

!wasm {
    QT += printsupport
}

macx {
    QT += svg
}

!wasm {
        LIBS += -L../addons -laddons
}

LIBS += -L../export -lexport
LIBS += -L../grades -lgrades
LIBS += -L../history -lhistory
LIBS += -L../updater -lupdater
include(../framework/framework.pri)

SOURCES += \
    App.cpp \
    AppMenuBar.cpp \
    AppModule.cpp \
    BuiltInPacks.cpp \
    internal/SplashScreen.cpp \
    main.cpp \
    settings/SettingsCategory.cpp

HEADERS += \
    App.h \
    AppMenuBar.h \
    AppModule.h \
    BuiltInPacks.h \
    internal/SplashScreen.h \
    settings/SettingsCategory.h

RESOURCES += \
    ../../res/icons/icons.qrc \
    ../../res/packs/packs.qrc \
    ../../res/splash/splash.qrc \
    ../../src/ui/qml/qml.qrc

RESOURCES += \
    ../../dark-theme/dark-style.qrc \
    ../../light-theme/light-style.qrc

TRANSLATIONS += \
    ../translations/Open-Typer_sk_SK.ts \
    ../translations/Open-Typer_de_DE.ts \
    ../translations/Open-Typer_ru_RU.ts

TRANSLATIONS += \
    ../translations/libcore_sk_SK.ts \
    ../translations/libcore_de_DE.ts \
    ../translations/libcore_ru_RU.ts

win32:RC_ICONS += ../../res/icons/icon.ico
macx:ICON = ../../res/macos-release/open-typer.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
