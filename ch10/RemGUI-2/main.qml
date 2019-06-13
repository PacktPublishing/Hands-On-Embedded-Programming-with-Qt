import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Extras 1.4

Window {
    id: mainWindow
    width: 600
    height: 400
    visible: true

    signal minSettingSignal(double value)
    signal maxSettingSignal(double value)

    property double currentTempValue
    property double maxSettingValue
    property double minSettingValue

    property bool fanOnValue
    property bool coolingOnValue
    property bool heatingOnValue

    onCurrentTempValueChanged: {currentTemp.text = currentTempValue}
    onMaxSettingValueChanged:  {rangeSetter.second.value = maxSettingValue}
    onMinSettingValueChanged:  {rangeSetter.first.value = minSettingValue}

    onFanOnValueChanged:     {fanInd.active = fanOnValue}
    onCoolingOnValueChanged: {coolingInd.active = coolingOnValue}
    onHeatingOnValueChanged: {headingInd.active = heatingOnValue}

    Page {
        id: currentConditions

        visible: true

        property alias currentTemp: currentTemp
        property alias maxSetting: maxSetting
        property alias minSetting: minSetting
        property alias rangeSetter: rangeSetter
        property alias fanInd: fanInd
        property alias coolingInd: coolingInd
        property alias heatingInd: heatingInd

        header: Label {
            text: qsTr("Current Conditions")
            font.pixelSize: Qt.application.font.pixelSize * 2
            padding: 10
        }

        StatusIndicator {
            id: heatingInd
            x: 55
            y: 47
            color: "#ff8484"
            active: false
        }

        StatusIndicator {
            id: coolingInd
            x: 55
            y: 83
            color: "#16fff3"
            active: false
        }

        StatusIndicator {
            id: fanInd
            x: 55
            y: 120
            color: "#ffcd69"
            active: false
        }

        Label {
            id: label
            x: 91
            y: 51
            text: qsTr("Heating")
            font.bold: true
            font.pointSize: 14
            horizontalAlignment: Text.AlignLeft
        }

        Label {
            id: label1
            x: 91
            y: 86
            text: qsTr("Cooling")
            font.pointSize: 15
            font.bold: true
        }

        Label {
            id: label2
            x: 90
            y: 123
            text: qsTr("Fan")
            font.pointSize: 14
            font.bold: true
        }

        RangeSlider {
            id: rangeSetter
            x: 162
            y: 210
            width: 277
            height: 61
            from: 0
            stepSize: 0.1
            to: 30
            wheelEnabled: true
            visible: true
            first.value: 12
            second.value: 15
            snapMode: RangeSlider.SnapAlways

            first.onValueChanged: {
                minSetting.text = first.value;
                mainWindow.minSettingSignal(first.value);
            }

            second.onValueChanged: {
                maxSetting.text = second.value;
                mainWindow.maxSettingSignal(second.value);
            }
        }

        Text {
            id: minSetting
            x: 162
            y: 277
            text: qsTr("min")
            font.pixelSize: 22
        }

        Text {
            id: maxSetting
            x: 390
            y: 277
            text: qsTr("max")
            font.pixelSize: 22
        }

        Text {
            id: currentTemp
            x: 341
            y: 62
            text: qsTr("15.2")
            font.family: "Arial"
            font.pixelSize: 64
        }
    }

}
