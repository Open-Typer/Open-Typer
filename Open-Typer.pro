TEMPLATE = subdirs

SUBDIRS += \
    app \
    libcore

app.depends = libcore
