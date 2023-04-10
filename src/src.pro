TEMPLATE = subdirs

SUBDIRS += \
    app \
    export \
    framework \
    grades \
    history \
    updater

app.depends = \
    export \
    framework \
    grades \
    history \
    updater

!wasm {
    SUBDIRS += addons
    app.depends += addons
}
