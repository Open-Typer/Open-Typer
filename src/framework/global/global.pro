TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

SOURCES += \
    GlobalModule.cpp \
    global.cpp \
    StringUtils.cpp \
    internal/FileUtils.cpp \
    internal/Settings.cpp

HEADERS += \
    GlobalModule.h \
    IFileUtils.h \
    ISettings.h \
    global.h \
    StringUtils.h \
    internal/FileUtils.h \
    internal/Settings.h \
    modularity/IModuleExportInterface.h \
    modularity/IModuleSetup.h \
    modularity/ModulesIoC.h \
    modularity/ioc.h
