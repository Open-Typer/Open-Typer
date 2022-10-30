QT += widgets network websockets charts

!wasm {
	QT += sql printsupport
	LIBS += -lssl -lcrypto
}

TEMPLATE = lib
TARGET = opentyper-core
DESTDIR = $$_PRO_FILE_PWD_/..

INCLUDEPATH += src/include

SOURCES += \
    src/AddonApi.cpp \
    src/BuiltInPacks.cpp \
    src/ConfigParser.cpp \
    src/ExportDialog.cpp \
    src/FileUtils.cpp \
    src/HistoryParser.cpp \
    src/KeyboardUtils.cpp \
    src/LanguageManager.cpp \
    src/LoadExerciseDialog.cpp \
    src/Settings.cpp \
    src/StatsDialog.cpp \
    src/StringUtils.cpp \
    src/widgets/TextView.cpp \
    src/ThemeEngine.cpp \
    src/IAddon.cpp

HEADERS += \
    src/include/AddonApi.h \
    src/include/BuiltInPacks.h \
    src/include/ConfigParser.h \
    src/include/ExportDialog.h \
    src/include/FileUtils.h \
    src/include/HistoryParser.h \
    src/include/KeyboardUtils.h \
    src/include/LanguageManager.h \
    src/include/LoadExerciseDialog.h \
    src/include/Settings.h \
    src/include/StatsDialog.h \
    src/include/StringUtils.h \
    src/include/widgets/TextView.h \
    src/include/ThemeEngine.h \
    src/include/IAddon.h

FORMS += \
    ui/ExportDialog.ui \
    ui/LoadExerciseDialog.ui \
    ui/StatsDialog.ui

DEFINES += CORE_SHARED_LIB

# Third-party
wasm {
	include($$PWD/../thirdparty/QWasmSettings/qwasmsettings.pri)
}