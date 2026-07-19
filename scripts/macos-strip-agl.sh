#!/usr/bin/env bash
# Strip the removed AGL framework from a Qt installation (macOS).
#
# Apple removed the legacy AGL framework from recent SDKs (Xcode 16+/macOS 15+),
# but Qt 6.8.x still references `-framework AGL` in its mkspecs and, crucially, in
# the per-framework .prl files (QtGui and friends — including the copies inside
# each *.framework/Versions/A/Resources). qmake expands those .prl at link time, so
# the app fails with: `ld: framework 'AGL' not found`. Selene renders with
# Metal/QuartzCore, not AGL, so removing it is safe.
#
# Usage: scripts/macos-strip-agl.sh [QTDIR]
#   QTDIR defaults to $Qt6_DIR (set by jurplel/install-qt-action) or a Homebrew Qt.
#
# NOTE: intentionally NOT `set -e`. The verification `grep -l` exits non-zero when
# it finds nothing — i.e. exactly when the strip SUCCEEDED — so `set -e` would abort
# the script precisely on success. We check the count explicitly instead.
set -uo pipefail

QTDIR="${1:-${Qt6_DIR:-}}"
if [ -z "$QTDIR" ] || [ ! -d "$QTDIR/lib" ]; then
  echo "macos-strip-agl: could not find a Qt lib dir (QTDIR='$QTDIR')" >&2
  exit 1
fi

strip='s/-framework AGL //g; s/-framework AGL;//g; s/-framework AGL$//g'

# Every .prl under lib/, including the ones inside each *.framework/Resources.
find "$QTDIR/lib" -name '*.prl' -print0 | xargs -0 sed -i '' -e "$strip"

# The OpenGL mkspec vars, for good measure.
for f in "$QTDIR/mkspecs/common/mac.conf" "$QTDIR/mkspecs/modules/qt_lib_gui_private.pri"; do
  if [ -f "$f" ]; then sed -i '' -e "$strip" "$f"; fi
done

# Verify. grep exits 1 on "no match" (the success case) — that's fine here because
# we are not under `set -e`; we read the count and decide explicitly.
left="$(find "$QTDIR/lib" -name '*.prl' -print0 | xargs -0 grep -l 'framework AGL' 2>/dev/null | wc -l | tr -d ' ')"
echo "macos-strip-agl: done ($QTDIR) — .prl still referencing AGL: ${left:-0}"
if [ "${left:-0}" != "0" ]; then
  echo "macos-strip-agl: ERROR: AGL still present in ${left} .prl file(s) after strip" >&2
  exit 1
fi
