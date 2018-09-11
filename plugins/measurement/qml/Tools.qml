import QtQuick 2.0
import QtQuick.Controls 1.4
import Measurement 1.0
Item {
    Column{
        anchors.horizontalCenter: parent.horizontalCenter
        y: 100
        Button{
            text: "Slice"
            onClicked: {
                GMouse.clickTool(String(sliceTool))
                SubBackends.workMode("pick")
            }
        }
        Button{
            text: "扫描"
            onClicked: {
                GMouse.clickTool(String(this))
                SubBackends.workMode("scan")
            }
        }
        spacing: 10
    }

    GridView {
        id: measureValues
        width: 100;
        height: cellHeight*count
        y: 200
        anchors.horizontalCenter: parent.horizontalCenter
        cellWidth: 100; cellHeight: 30

        model:ListModel{
            id: msModel
            property real ms_index
            ListElement{name: "身高";value: ""}
            ListElement{name: "胸围";value: ""}
            ListElement{name: "腰围";value: ""}
            ListElement{name: "臀围";value: ""}
            ListElement{name: "会阴高";value: ""}
        }
        property var textField

        delegate: Row{
            Text{
                width: measureValues.cellWidth*0.4
                text: name + ":"
            }
            TextField{
                id: itemValue
                width: measureValues.cellWidth*0.6
                text: value
                onFocusChanged: {
                    if(focus) msModel.ms_index = model.index
                }

                Connections{
                    target: SubBackends
                    onMeasureValueUpdate:
                        msModel.setProperty(valueId, "value", ms_value)
                }
            }
        }
        focus: true
    }

    Connections{
        id: sliceTool
        target: GMouse
        enabled: GMouse.current_tool === String(sliceTool)
        onClicked:{
            SubBackends.slice(mouse.x*devicePixelRatio,mouse.y*devicePixelRatio)
        }
    }

    Text {
        anchors.top: parent.top
        anchors.topMargin: 60
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        text: "周长: "
        id: girthLable
        Connections{
            target: SubBackends
            enabled: msModel.ms_index >= 1 && msModel.ms_index <= 3
            onSliceUpdate:{
                girthLable.text = "周长: "+ sliceValue
                if(msModel.ms_index >= 1) {
                    msModel.setProperty(msModel.ms_index, "value", sliceValue)
                }
            }
        }
    }

    Rectangle{
        anchors.left: parent.right
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.leftMargin: 10
        width:  400
        height: 400
        color: Qt.rgba(0.3,0.3,0,0.2)
        SliceCanvas{
            id: slice
            anchors.fill: parent
            Component.onCompleted: {
                SubBackends.setSliceCanvas(this)
            }
            onSliceChanged:  slice.update()
        }
    }
}
