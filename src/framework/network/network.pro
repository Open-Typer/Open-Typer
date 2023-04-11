TEMPLATE = lib
QT += core network
CONFIG += staticlib

HEADERS += \
        INetUtils.h \
        NetworkModule.h \
        internal/NetUtils.h

SOURCES += \
        NetworkModule.cpp \
        internal/NetUtils.cpp
