QT += widgets network websockets

!wasm {
	QT += sql printsupport
	LIBS += -lssl -lcrypto
}

TEMPLATE = lib
TARGET = opentyper-core
DESTDIR = $$_PRO_FILE_PWD_/..

INCLUDEPATH += src/include

SOURCES += \
    src/BuiltInPacks.cpp \
    src/ConfigParser.cpp \
    src/DatabaseManager.cpp \
    src/ExportDialog.cpp \
    src/FileUtils.cpp \
    src/HistoryParser.cpp \
    src/KeyboardUtils.cpp \
    src/LanguageManager.cpp \
    src/MonitorClient.cpp \
    src/LoadExerciseDialog.cpp \
    src/Settings.cpp \
    src/StringUtils.cpp \
    src/widgets/TextView.cpp \
    src/ThemeEngine.cpp \
    src/IAddon.cpp

!wasm {
	SOURCES += src/MonitorServer.cpp
}

HEADERS += \
    src/include/BuiltInPacks.h \
    src/include/ConfigParser.h \
    src/include/DatabaseManager.h \
    src/include/ExportDialog.h \
    src/include/FileUtils.h \
    src/include/HistoryParser.h \
    src/include/KeyboardUtils.h \
    src/include/LanguageManager.h \
    src/include/MonitorClient.h \
    src/include/LoadExerciseDialog.h \
    src/include/Settings.h \
    src/include/StringUtils.h \
    src/include/widgets/TextView.h \
    src/include/ThemeEngine.h \
    src/include/IAddon.h

!wasm {
	HEADERS += src/include/MonitorServer.h
}

FORMS += \
    ui/ExportDialog.ui \
    ui/LoadExerciseDialog.ui

DEFINES += CORE_SHARED_LIB

# Third-party
wasm {
	include($$PWD/../thirdparty/QWasmSettings/qwasmsettings.pri)
}
