QT += widgets charts quick

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

TEMPLATE = lib
TARGET = opentyper-core
DESTDIR = $$_PRO_FILE_PWD_/..

INCLUDEPATH += src

SOURCES += \
    src/AddonApi.cpp \
    src/AddonButton.cpp \
    src/AddonSettingsCategory.cpp \
    src/AbstractMenuBar.cpp \
    src/AppMenuBar.cpp \
    src/AppMenuItem.cpp \
    src/AppMenuModel.cpp \
    src/BuiltInPacks.cpp \
    src/CharacterRecord.cpp \
    src/ConfigParser.cpp \
    src/ExerciseTimer.cpp \
    src/ExerciseValidator.cpp \
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
    src/StringUtils.cpp \
    src/ThemeEngine.cpp \
    src/IAddon.cpp

HEADERS += \
    src/AddonApi.h \
    src/AddonButton.h \
    src/AddonSettingsCategory.h \
    src/AbstractMenuBar.h \
    src/AppMenuBar.h \
    src/AppMenuItem.h \
    src/AppMenuModel.h \
    src/BuiltInPacks.h \
    src/CharacterRecord.h \
    src/ConfigParser.h \
    src/ExerciseTimer.h \
    src/ExerciseValidator.h \
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
    src/StringUtils.h \
    src/ThemeEngine.h \
    src/IAddon.h

TRANSLATIONS += \
    translations/libcore_sk_SK.ts \
    translations/libcore_de_DE.ts \
    translations/libcore_ru_RU.ts

RESOURCES += \
    translations/core-translations.qrc

DEFINES += CORE_SHARED_LIB

# Third party
include(../3rdparty/xkeyboard-config/xkeyboard-config.pri)
include(../3rdparty/libxkbcommon/libxkbcommon.pri)
