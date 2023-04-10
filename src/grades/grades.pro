TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    GradesModule.cpp \
    internal/Class.cpp \
    internal/ClassManager.cpp \
    internal/GradeCalculator.cpp

HEADERS += \
    GradesModule.h \
    IClassManager.h \
    internal/Class.h \
    internal/ClassManager.h \
    internal/GradeCalculator.h

RESOURCES += \
        grades.qrc
