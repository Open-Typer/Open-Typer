TEMPLATE = subdirs

SUBDIRS += \
    global \
    lessonpack \
    keyboard \
    translations \
    ui \
    uicomponents \
    utils \
    validator

!wasm {
    SUBDIRS += network
}
