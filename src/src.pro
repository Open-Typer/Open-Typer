TEMPLATE = subdirs

SUBDIRS += \
    app \
    addons \
    export \
    framework \
    grades \
    history \
    updater

app.depends = \
    addons \
    export \
    framework \
    grades \
    history \
    updater
