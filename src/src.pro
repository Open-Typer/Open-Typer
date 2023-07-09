TEMPLATE = subdirs

SUBDIRS += \
    app \
    export \
    framework \
    grades \
    history \
    packeditor \
    updater

app.depends = \
    export \
    framework \
    grades \
    history \
    updater
