#-------------------------------------------------
#
# Project created by QtCreator 2019-02-05T16:54:48
#
#-------------------------------------------------

QT += core gui widgets quick quickwidgets sql scxml websockets virtualkeyboard sensors mqtt

TARGET = BigProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    TemperatureSensorIF.cpp \
    MockTempSensor.cpp \
    TemperatureHistoryForm.cpp \
    TemperatureReading.cpp \
    TemperatureStorage.cpp \
    HVACController.cpp \
    WSReporter.cpp \
    WeatherFetcher.cpp \
    AmbientTempSensor.cpp \
    MqttClient.cpp

HEADERS += \
        MainWindow.h \
    TemperatureSensorIF.h \
    MockTempSensor.h \
    TemperatureHistoryForm.h \
    TemperatureReading.h \
    TemperatureStorage.h \
    HVACController.h \
    WSReporter.h \
    WeatherFetcher.h \
    AmbientTempSensor.h \
    MqttClient.h

FORMS += \
        MainWindow.ui \
    TemperatureHistoryForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc

STATECHARTS += \
    HVACStateMachine.scxml

DISTFILES += \
    html/monitor.html \
    InputPanelArea.qml \
    InputPanelArea.qml
