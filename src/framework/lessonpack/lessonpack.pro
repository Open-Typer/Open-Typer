TEMPLATE = lib
QT += core qml
CONFIG += staticlib

SOURCES += \
    LessonPackModule.cpp \
    internal/ConfigParser.cpp

HEADERS += \
    IConfigParser.h \
    LessonPackModule.h \
    internal/ConfigParser.h
