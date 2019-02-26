import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.VirtualKeyboard 2.4

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello from QML!")

    ColumnLayout {
        id: verticalLayout
        anchors.fill: parent

        Label {
            id: greetings
            text: qsTr("Hello from QML!")

            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            color: "red"
            font.bold: true
            font.pointSize: 20
        }

        RowLayout {
            id: horizontalLayout
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Label {
                id: nameLabel
                text: qsTr("My Name is:")

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            TextField {
                id: nameField
                text: qsTr("")

                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                onAccepted: {
                    greetings.text = "Nice to meet you " + nameField.text + "!"
                }
            }
        }

        RoundButton {
            id: button
            text: qsTr("Enter Name")

            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            onClicked: {
                greetings.text = "Nice to meet you " + nameField.text + "!"
            }
        }
    }
}


