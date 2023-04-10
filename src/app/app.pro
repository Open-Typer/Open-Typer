TEMPLATE = app
TARGET = open-typer

QT += core gui widgets quick quickcontrols2
QTPLUGIN += qsvg
CONFIG += lrelease
CONFIG += embed_translations

macx {
    PRE_TARGETDEPS += compiler_lrelease_make_all
}

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
    app.qrc \
    ../../res/icons/icons.qrc \
    ../../res/packs/packs.qrc

RESOURCES += \
    ../../res/styles/dark/dark-style.qrc \
    ../../res/styles/light/light-style.qrc

EXTRA_TRANSLATIONS += \
    ../../translations/Open-Typer_sk_SK.ts \
    ../../translations/Open-Typer_de_DE.ts \
    ../../translations/Open-Typer_ru_RU.ts

win32:RC_ICONS += ../../res/icons/icon.ico
macx:ICON = ../../res/macos-release/open-typer.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
