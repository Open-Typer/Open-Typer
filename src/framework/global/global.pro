TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

SOURCES += \
    FileUtils.cpp \
    GlobalModule.cpp \
    global.cpp \
    Settings.cpp \
    StringUtils.cpp

HEADERS += \
    FileUtils.h \
    GlobalModule.h \
    global.h \
    Settings.h \
    StringUtils.h \
    modularity/IModuleSetup.h
