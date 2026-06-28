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
}

# Enable ASan for Linux or macOS
#CONFIG += sanitizer sanitize_address

# Enable ASan for Windows
#QMAKE_CFLAGS += -fsanitize=address
#QMAKE_CXXFLAGS += -fsanitize=address
#QMAKE_LFLAGS += -incremental:no -wholearchive:clang_rt.asan_dynamic-x86_64.lib -wholearchive:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib
