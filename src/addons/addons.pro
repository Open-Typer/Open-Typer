TEMPLATE = lib
QT += core widgets qml network
CONFIG += staticlib

SOURCES += \
    AddonApi.cpp \
    AddonButton.cpp \
    AddonItemModel.cpp \
    AddonListModel.cpp \
    AddonManager.cpp \
    AddonModel.cpp

HEADERS += \
    AddonApi.h \
    AddonButton.h \
    AddonItemModel.h \
    AddonListModel.h \
    AddonManager.h \
    AddonModel.h \
    IAddon.h
