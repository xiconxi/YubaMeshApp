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
        Button{text: "test*"}
        Button{text: "test*"}
        spacing: 10
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
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        text: "身高: "
        id: heightLable
        Connections{
            target: SubBackends
            onHeightUpdate:{
                heightLable.text = "身高: "+ heightValue
            }
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
            onSliceUpdate:{
                girthLable.text = "周长: "+ sliceValue
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
