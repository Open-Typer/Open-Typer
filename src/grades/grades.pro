TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    Class.cpp \
    ClassManager.cpp \
    GradesModule.cpp \
    internal/GradeCalculator.cpp

HEADERS += \
    Class.h \
    ClassManager.h \
    GradesModule.h \
    internal/GradeCalculator.h

RESOURCES += \
        grades.qrc
