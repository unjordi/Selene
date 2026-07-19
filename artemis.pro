TEMPLATE = subdirs
SUBDIRS = \
    moonlight-common-c \
    qmdnsengine \
    app \
    h264bitstream

# Build the dependencies in parallel before the final app
app.depends = qmdnsengine moonlight-common-c h264bitstream
win32:!winrt {
    SUBDIRS += AntiHooking
    app.depends += AntiHooking
}
!winrt:win32|macx {
    SUBDIRS += soundio
    app.depends += soundio
}

# Unit tests (QtTest). Opt-in only: `qmake CONFIG+=test` — a normal build
# (qmake && make) ignores tests/ entirely. Run with `make check`.
test {
    SUBDIRS += tests
}

# Support debug and release builds from command line for CI
CONFIG += debug_and_release

# Run our compile tests
load(configure)
qtCompileTest(SL)
qtCompileTest(EGL)
