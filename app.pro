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
TARGET = opentyper-main
DESTDIR = $$_PRO_FILE_PWD_

INCLUDEPATH += src

!wasm {
    SOURCES += \
        src/AddonItemModel.cpp \
	src/AddonListModel.cpp \
	src/AddonManager.cpp \
	src/AddonModel.cpp
}

SOURCES += \
    src/AddonApi.cpp \
    src/AddonButton.cpp \
    src/AbstractMenuBar.cpp \
    src/AppMenuBar.cpp \
    src/AppMenuItem.cpp \
    src/AppMenuModel.cpp \
    src/BuiltInPacks.cpp \
    src/CharacterRecord.cpp \
    src/ClassManager.cpp \
    src/Class.cpp \
    src/ConfigParser.cpp \
    src/ExerciseTimer.cpp \
    src/ExerciseValidator.cpp \
    src/ExportProvider.cpp \
    src/ExportTable.cpp \
    src/ExportTableModel.cpp \
    src/QmlWidget.cpp \
    src/packEditor/PackEditor.cpp \
    src/packEditor/PackSelector.cpp \
    src/updater/Updater.cpp \
    src/FileUtils.cpp \
    src/HistoryEntry.cpp \
    src/HistoryParser.cpp \
    src/Key.cpp \
    src/KeyboardLayout.cpp \
    src/KeyboardUtils.cpp \
    src/LanguageManager.cpp \
    src/MistakeRecord.cpp \
    src/QmlFileDialog.cpp \
    src/QmlKeyboardHandler.cpp \
    src/QmlUtils.cpp \
    src/Settings.cpp \
    src/SettingsCategory.cpp \
    src/StringUtils.cpp \
    src/ThemeEngine.cpp \
    src/global.cpp

!wasm {
    HEADERS += \
        src/AddonItemModel.h \
	src/AddonListModel.h \
	src/AddonManager.h \
	src/AddonModel.h
}

HEADERS += \
    src/AddonApi.h \
    src/AddonButton.h \
    src/AbstractMenuBar.h \
    src/AppMenuBar.h \
    src/AppMenuItem.h \
    src/AppMenuModel.h \
    src/BuiltInPacks.h \
    src/CharacterRecord.h \
    src/ClassManager.h \
    src/Class.h \
    src/ConfigParser.h \
    src/ExerciseTimer.h \
    src/ExerciseValidator.h \
    src/ExportProvider.h \
    src/ExportTable.h \
    src/ExportTableModel.h \
    src/QmlWidget.h \
    src/packEditor/PackEditor.h \
    src/packEditor/PackSelector.h \
    src/updater/Updater.h \
    src/FileUtils.h \
    src/HistoryEntry.h \
    src/HistoryParser.h \
    src/Key.h \
    src/KeyboardLayout.h \
    src/KeyboardUtils.h \
    src/LanguageManager.h \
    src/MistakeRecord.h \
    src/QmlFileDialog.h \
    src/QmlKeyboardHandler.h \
    src/QmlUtils.h \
    src/Settings.h \
    src/SettingsCategory.h \
    src/StringUtils.h \
    src/ThemeEngine.h \
    src/IAddon.h \
    src/global.h

FORMS += \
    src/packEditor/PackEditor.ui \
    src/packEditor/PackSelector.ui

TRANSLATIONS += \
    translations/Open-Typer_sk_SK.ts \
    translations/Open-Typer_de_DE.ts \
    translations/Open-Typer_ru_RU.ts

TRANSLATIONS += \
    translations/libcore_sk_SK.ts \
    translations/libcore_de_DE.ts \
    translations/libcore_ru_RU.ts

RESOURCES += \
    translations/core-translations.qrc

# Third party
include(3rdparty/xkeyboard-config/xkeyboard-config.pri)
include(3rdparty/libxkbcommon/libxkbcommon.pri)
