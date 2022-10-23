QT += widgets network websockets

!wasm {
	QT += sql
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
    src/FileUtils.cpp \
    src/HistoryParser.cpp \
    src/KeyboardUtils.cpp \
    src/LanguageManager.cpp \
    src/MonitorClient.cpp \
    src/MonitorServer.cpp \
    src/Settings.cpp \
    src/StringUtils.cpp \
    src/ThemeEngine.cpp

HEADERS += \
    src/include/BuiltInPacks.h \
    src/include/ConfigParser.h \
    src/include/DatabaseManager.h \
    src/include/FileUtils.h \
    src/include/HistoryParser.h \
    src/include/KeyboardUtils.h \
    src/include/LanguageManager.h \
    src/include/MonitorClient.h \
    src/include/MonitorServer.h \
    src/include/Settings.h \
    src/include/StringUtils.h \
    src/include/ThemeEngine.h

DEFINES += CORE_SHARED_LIB

# Third-party
wasm {
	include($$PWD/../thirdparty/QWasmSettings/qwasmsettings.pri)
}
