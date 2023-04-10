TEMPLATE = lib
QT += core widgets qml network
CONFIG += staticlib

QML_IMPORT_PATH += qml

SOURCES += \
    AddonsModule.cpp \
    internal/AddonApi.cpp \
    internal/AddonButton.cpp \
    internal/AddonItemModel.cpp \
    internal/AddonListModel.cpp \
    internal/AddonManager.cpp \
    internal/AddonModel.cpp

HEADERS += \
    AddonsModule.h \
    IAddon.h \
    IAddonApi.h \
    IAddonManager.h \
    internal/AddonApi.h \
    internal/AddonButton.h \
    internal/AddonItemModel.h \
    internal/AddonListModel.h \
    internal/AddonManager.h \
    internal/AddonModel.h

RESOURCES += \
        addons.qrc
