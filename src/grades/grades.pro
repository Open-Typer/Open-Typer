TEMPLATE = lib
QT += core widgets qml
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    Class.cpp \
    GradesModule.cpp \
    internal/ClassManager.cpp \
    internal/GradeCalculator.cpp

HEADERS += \
    Class.h \
    GradesModule.h \
    IClassManager.h \
    internal/ClassManager.h \
    internal/GradeCalculator.h

RESOURCES += \
        grades.qrc
