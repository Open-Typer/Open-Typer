TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

SOURCES += \
    FileUtils.cpp \
    global.cpp \
    Settings.cpp \
    StringUtils.cpp

HEADERS += \
    FileUtils.h \
    global.h \
    Settings.h \
    StringUtils.h \
    modularity/IModuleSetup.h
