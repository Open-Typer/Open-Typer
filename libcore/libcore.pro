QT += widgets charts quick

linux {
        QT += dbus
}

!wasm {
	QT += printsupport
	LIBS += -lssl -lcrypto
}

CONFIG += c++17

TEMPLATE = lib
TARGET = opentyper-core
!win32 {
	VERSION = $$system(git describe --tags --abbrev=0 | sed -e 's/v//g')
}
DESTDIR = $$_PRO_FILE_PWD_/..

INCLUDEPATH += src/include

SOURCES += \
    src/AddonApi.cpp \
    src/BuiltInPacks.cpp \
    src/CharacterRecord.cpp \
    src/ConfigParser.cpp \
    src/ExerciseTimer.cpp \
    src/ExerciseValidator.cpp \
    src/ExportDialog.cpp \
    src/FileUtils.cpp \
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
    src/widgets/TextView.cpp \
    src/ThemeEngine.cpp \
    src/IAddon.cpp

HEADERS += \
    src/include/AddonApi.h \
    src/include/BuiltInPacks.h \
    src/include/CharacterRecord.h \
    src/include/ConfigParser.h \
    src/include/ExerciseTimer.h \
    src/include/ExerciseValidator.h \
    src/include/ExportDialog.h \
    src/include/FileUtils.h \
    src/include/HistoryParser.h \
    src/include/Key.h \
    src/include/KeyboardLayout.h \
    src/include/KeyboardUtils.h \
    src/include/LanguageManager.h \
    src/include/MistakeRecord.h \
    src/include/QmlFileDialog.h \
    src/include/QmlKeyboardHandler.h \
    src/include/QmlUtils.h \
    src/include/Settings.h \
    src/include/StringUtils.h \
    src/include/widgets/TextView.h \
    src/include/ThemeEngine.h \
    src/include/IAddon.h

FORMS += \
    ui/ExportDialog.ui

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
