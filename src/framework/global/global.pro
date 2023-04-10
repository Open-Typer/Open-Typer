TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

SOURCES += \
    GlobalModule.cpp \
    internal/FileUtils.cpp \
    internal/Settings.cpp \
    internal/StringUtils.cpp

HEADERS += \
    GlobalModule.h \
    IFileUtils.h \
    ISettings.h \
    IStringUtils.h \
    global.h \
    internal/FileUtils.h \
    internal/Settings.h \
    internal/StringUtils.h \
    modularity/IModuleExportInterface.h \
    modularity/IModuleSetup.h \
    modularity/ModulesIoC.h \
    modularity/ioc.h
