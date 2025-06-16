QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_parsingtest.cpp \
    ../rawreport.cpp \
    ../lib/profile.cpp \
    ../lib/logline.cpp

HEADERS += \
    ../rawreport.h \
    ../lib/profile.h \
    ../lib/logline.h

INCLUDEPATH += ../
