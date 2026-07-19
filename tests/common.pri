# Shared config for Selene's QtTest unit tests.
QT += testlib
QT -= gui
CONFIG += console testcase c++17
CONFIG -= app_bundle
TEMPLATE = app

# Repo root so tests can include app/ headers as "utils/...".
INCLUDEPATH += $$PWD/../app
