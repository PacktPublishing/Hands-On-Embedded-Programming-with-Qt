include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt core

HEADERS += \
        tst_updatestatemachinefromtemperature.h \
    ../BigProject/HVACController.h

SOURCES += \
        main.cpp \
    ../BigProject/HVACController.cpp

INCLUDEPATH += ../BigProject
