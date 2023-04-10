TEMPLATE = lib
QT += core qml
CONFIG += staticlib

SOURCES += \
    TranslationsModule.cpp \
    internal/LanguageManager.cpp

HEADERS += \
    ILanguageManager.h \
    TranslationsModule.h \
    internal/LanguageManager.h
