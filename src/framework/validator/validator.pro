TEMPLATE = lib
QT += core widgets qml
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
