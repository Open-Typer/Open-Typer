TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

SOURCES += \
    FileUtils.cpp \
    GlobalModule.cpp \
    global.cpp \
    StringUtils.cpp \
    internal/Settings.cpp

HEADERS += \
    FileUtils.h \
    GlobalModule.h \
    ISettings.h \
    global.h \
    StringUtils.h \
    internal/Settings.h \
    modularity/IModuleExportInterface.h \
    modularity/IModuleSetup.h \
    modularity/ModulesIoC.h \
    modularity/ioc.h
