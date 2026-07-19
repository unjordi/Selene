#pragma once

// Pure refresh-rate parsing, locale-independent. Mirrors the fix in
// SettingsView.qml (#65): accept both '.' and ',' as the decimal separator so
// fractional rates like 59.94 are never rejected or truncated under a
// comma-decimal locale. Extracted as a header so it can be unit-tested.

#include <QString>
#include <QLocale>

namespace RefreshRate {

  // Parse a refresh rate, accepting "59.94" and "59,94" regardless of the
  // process locale. Returns true and writes *out on success (rate > 0).
  inline bool parse(const QString &text, double *out) {
    QString s = text.trimmed();
    if (s.isEmpty()) {
      return false;
    }
    s.replace(',', '.');  // normalize decimal separator
    bool ok = false;
    const double v = QLocale::c().toDouble(s, &ok);  // QLocale::c() => '.' always
    if (!ok || v <= 0.0) {
      return false;
    }
    if (out) {
      *out = v;
    }
    return true;
  }

  // Milli-hertz representation sent to the host (mirrors session.cpp).
  inline int toMilliHz(double hz) {
    return static_cast<int>(hz * 1000);
  }

}  // namespace RefreshRate
