TEMPLATE = lib
QT += core qml
CONFIG += staticlib

SOURCES += \
    ValidatorModule.cpp \
    internal/ExerciseValidator.cpp

HEADERS += \
    CharacterRecord.h \
    IExerciseValidator.h \
    MistakeRecord.h \
    ValidatorModule.h \
    internal/ExerciseValidator.h
