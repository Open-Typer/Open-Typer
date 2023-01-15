TARGET = open-typer
DESTDIR = $$_PRO_FILE_PWD_/..

QT += core gui quick quickcontrols2 charts
QTPLUGIN += qsvg
!wasm {
    QT += printsupport
    LIBS += -lssl -lcrypto
}
macx {
    QT += svg
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Version and revision
win32 {
    GIT_LOCATION = $$system(where git)
}
!win32 {
    GIT_LOCATION = $$system(which git)
}
isEmpty(GIT_LOCATION) | !exists(../.git) {
    exists(version) : exists(revision) {
        DEFINES += BUILD_VERSION=\\\"$$cat(version)\\\"
        DEFINES += BUILD_REVISION=\\\"$$cat(revision)\\\"
    }
} else {
    DEFINES += BUILD_VERSION=\\\"$$system(git describe --tags --abbrev=0)\\\"
    DEFINES += BUILD_REVISION=\\\"$$system(git rev-parse --short HEAD)\\\"
}

INCLUDEPATH += \
    src/include \
    ../libcore/src/include

LIBS += -L$$_PRO_FILE_PWD_/.. -lopentyper-core

SOURCES += \
    src/packEditor/PackEditor.cpp \
    src/packEditor/PackSelector.cpp \
    src/updater/Updater.cpp \
    src/main.cpp \
    src/widgets/KeyboardWidget.cpp

HEADERS += \
    src/include/packEditor/PackEditor.h \
    src/include/packEditor/PackSelector.h \
    src/include/updater/Updater.h \
    src/include/widgets/KeyboardWidget.h

FORMS += \
    ui/packEditor/PackEditor.ui \
    ui/packEditor/PackSelector.ui

DISTFILES += \
    ui/qml/controls/AccentButton.qml \
    ui/qml/controls/AccentButtonBox.qml \
    ui/qml/controls/ColorButton.qml \
    ui/qml/controls/CustomComboBox.qml \
    ui/qml/controls/CustomDialog.qml \
    ui/qml/controls/CustomMenu.qml \
    ui/qml/controls/CustomMenuBar.qml \
    ui/qml/controls/CustomMenuItem.qml \
    ui/qml/controls/CustomMenuSeparator.qml \
    ui/qml/controls/CustomToolButton.qml \
    ui/qml/controls/FontComboBox.qml \
    ui/qml/controls/HoverToolTip.qml \
    ui/qml/controls/MenuButton.qml \
    ui/qml/controls/Panel.qml \
    ui/qml/controls/UpdateQuestion.qml \
    ui/qml/controls/VerticalStackView.qml \
    ui/qml/dialogs/ExerciseSummary.qml \
    ui/qml/dialogs/InitialSetup.qml \
    ui/qml/dialogs/MessageBox.qml \
    ui/qml/dialogs/SettingsDialog.qml \
    ui/qml/dialogs/TimeDialog.qml \
    ui/qml/dialogs/TypingTestDialog.qml \
    ui/qml/KeyboardLayoutList.qml \
    ui/qml/LanguageList.qml \
    ui/qml/LessonPackList.qml \
    ui/qml/MainWindow.qml \
    ui/qml/Paper.qml \
    ui/qml/settings/AppearanceSettings.qml \
    ui/qml/settings/BehaviorSettings.qml \
    ui/qml/settings/KeyboardSettings.qml \
    ui/qml/settings/LanguageSettings.qml

TRANSLATIONS += \
    translations/Open-Typer_sk_SK.ts \
    translations/Open-Typer_de_DE.ts \
    translations/Open-Typer_ru_RU.ts

RESOURCES += \
    res.qrc \
    res/icons/icons.qrc \
    res/images/class-icons/class-icons.qrc \
    res/keyboard-layouts/layouts.qrc \
    translations/translations.qrc \
    ui/qml/qml.qrc

RESOURCES += \
    dark-theme/dark-style.qrc \
    light-theme/light-style.qrc

win32:RC_ICONS += res/images/icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
