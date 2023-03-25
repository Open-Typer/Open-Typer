QT += core gui widgets quick quickcontrols2 charts network
CONFIG += qtquickcompiler

linux {
    QT += dbus
}

!wasm {
    QT += printsupport
    LIBS += -lssl -lcrypto
}

win32 {
    VERSION=""
}

CONFIG += c++17
CONFIG += shared

TEMPLATE = lib
TARGET = opentyper-core
DESTDIR = $$_PRO_FILE_PWD_/..

INCLUDEPATH += src

!wasm {
    SOURCES += \
        AddonItemModel.cpp \
	AddonListModel.cpp \
	AddonManager.cpp \
	AddonModel.cpp
}

SOURCES += \
    AddonApi.cpp \
    AddonButton.cpp \
    AbstractMenuBar.cpp \
    AppMenuBar.cpp \
    AppMenuItem.cpp \
    AppMenuModel.cpp \
    BuiltInPacks.cpp \
    CharacterRecord.cpp \
    ClassManager.cpp \
    Class.cpp \
    ConfigParser.cpp \
    ExerciseTimer.cpp \
    ExerciseValidator.cpp \
    ExportProvider.cpp \
    ExportTable.cpp \
    ExportTableModel.cpp \
    QmlWidget.cpp \
    packEditor/PackEditor.cpp \
    packEditor/PackSelector.cpp \
    updater/Updater.cpp \
    FileUtils.cpp \
    HistoryEntry.cpp \
    HistoryParser.cpp \
    Key.cpp \
    KeyboardLayout.cpp \
    KeyboardUtils.cpp \
    LanguageManager.cpp \
    MistakeRecord.cpp \
    QmlFileDialog.cpp \
    QmlKeyboardHandler.cpp \
    QmlUtils.cpp \
    Settings.cpp \
    SettingsCategory.cpp \
    StringUtils.cpp \
    ThemeEngine.cpp \
    global.cpp

!wasm {
    HEADERS += \
        AddonItemModel.h \
	AddonListModel.h \
	AddonManager.h \
	AddonModel.h
}

HEADERS += \
    AddonApi.h \
    AddonButton.h \
    AbstractMenuBar.h \
    AppMenuBar.h \
    AppMenuItem.h \
    AppMenuModel.h \
    BuiltInPacks.h \
    CharacterRecord.h \
    ClassManager.h \
    Class.h \
    ConfigParser.h \
    ExerciseTimer.h \
    ExerciseValidator.h \
    ExportProvider.h \
    ExportTable.h \
    ExportTableModel.h \
    QmlWidget.h \
    packEditor/PackEditor.h \
    packEditor/PackSelector.h \
    updater/Updater.h \
    FileUtils.h \
    HistoryEntry.h \
    HistoryParser.h \
    Key.h \
    KeyboardLayout.h \
    KeyboardUtils.h \
    LanguageManager.h \
    MistakeRecord.h \
    QmlFileDialog.h \
    QmlKeyboardHandler.h \
    QmlUtils.h \
    Settings.h \
    SettingsCategory.h \
    StringUtils.h \
    ThemeEngine.h \
    IAddon.h \
    global.h

FORMS += \
    packEditor/PackEditor.ui \
    packEditor/PackSelector.ui

TRANSLATIONS += \
    ../translations/Open-Typer_sk_SK.ts \
    ../translations/Open-Typer_de_DE.ts \
    ../translations/Open-Typer_ru_RU.ts

TRANSLATIONS += \
    ../translations/libcore_sk_SK.ts \
    ../translations/libcore_de_DE.ts \
    ../translations/libcore_ru_RU.ts

RESOURCES += \
    ../translations/core-translations.qrc

# Third party
include(../3rdparty/xkeyboard-config/xkeyboard-config.pri)
include(../3rdparty/libxkbcommon/libxkbcommon.pri)
