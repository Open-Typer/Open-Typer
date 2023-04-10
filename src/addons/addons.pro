TEMPLATE = lib
QT += core widgets qml network
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    AddonButton.cpp \
    AddonItemModel.cpp \
    AddonListModel.cpp \
    AddonModel.cpp \
    AddonsModule.cpp \
    internal/AddonApi.cpp \
    internal/AddonManager.cpp

HEADERS += \
    AddonButton.h \
    AddonItemModel.h \
    AddonListModel.h \
    AddonModel.h \
    AddonsModule.h \
    IAddon.h \
    IAddonApi.h \
    IAddonManager.h \
    internal/AddonApi.h \
    internal/AddonManager.h

RESOURCES += \
        addons.qrc
