TEMPLATE = lib
QT += core widgets qml network
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    AddonApi.cpp \
    AddonButton.cpp \
    AddonItemModel.cpp \
    AddonListModel.cpp \
    AddonModel.cpp \
    AddonsModule.cpp \
    internal/AddonManager.cpp

HEADERS += \
    AddonApi.h \
    AddonButton.h \
    AddonItemModel.h \
    AddonListModel.h \
    AddonModel.h \
    AddonsModule.h \
    IAddon.h \
    IAddonManager.h \
    internal/AddonManager.h

RESOURCES += \
        addons.qrc
