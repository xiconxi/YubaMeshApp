import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2
Button {
    property string imgsource: " "
    property color text_color: "blue"
    implicitHeight: 50
    implicitWidth: 50
    style: ButtonStyle {
        label: Item {
            anchors.centerIn: parent
            anchors.fill: parent
            Image {
                source: control.imgsource === " "?"/icons/sample.png":control.imgsource
                anchors.top: parent.top
                anchors.fill: parent
                opacity: 0.8
            }
            Text {
                text: control.text
                color: text_color
                anchors.bottom: parent.bottom
                anchors.horizontalCenter : parent.horizontalCenter
            }
        }
        background: Rectangle {
            radius: 5
            border.width: control.activeFocus ? 5 : 0
            gradient: hovered?Gradients.gd2:Gradients.gd6
        }
    }
}
