# Support debug and release builds from command line for CI
CONFIG += debug_and_release

# Ensure symbols are always generated
CONFIG += force_debug_info

# Disable asserts on release builds
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
}

# macOS: Xcode 16+/26 clang promotes implicit function declarations to errors.
# Qt 6.8.3's qyieldcpu.h declares the ARM __yield intrinsic implicitly, which
# breaks the build inside Qt's own header (-Werror,-Wimplicit-function-declaration).
# Downgrade it to a warning on macOS so we compile. Remove once Qt ships a fixed header.
macx {
    QMAKE_CFLAGS   += -Wno-error=implicit-function-declaration
    QMAKE_CXXFLAGS += -Wno-error=implicit-function-declaration

    # NOTE on AGL: Apple removed the legacy AGL framework from recent SDKs
    # (Xcode 16+/macOS 15+), but Qt 6.8.3 still references `-framework AGL`. It is
    # NOT reachable from here: it comes in through Qt's per-framework .prl files
    # (QtGui and friends, incl. the copies inside each *.framework/Resources), which
    # qmake expands at link time -> "ld: framework 'AGL' not found". A
    # `QMAKE_LIBS_OPENGL -= -framework AGL` in this scope does nothing about that.
    # The fix is to strip AGL from the Qt *installation* before building: CI does it
    # in .github/workflows/dev-build.yml; for a local macOS build run
    # scripts/macos-strip-agl.sh against your Qt dir. Remove once Qt stops shipping AGL.
}

# Enable ASan for Linux or macOS
#CONFIG += sanitizer sanitize_address

# Enable ASan for Windows
#QMAKE_CFLAGS += -fsanitize=address
#QMAKE_CXXFLAGS += -fsanitize=address
#QMAKE_LFLAGS += -incremental:no -wholearchive:clang_rt.asan_dynamic-x86_64.lib -wholearchive:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib
