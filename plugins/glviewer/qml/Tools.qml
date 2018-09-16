import QtQuick 2.0
import QtQuick.Controls 1.4
import GLViewer 1.0
import QtQuick.Controls.Styles 1.4

Item {
//    visible: false
    SelectCanvas{
        id: selectCanvas
        anchors.fill: parent
    }

    Rectangle{
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.6
        height: 30
        color: Qt.rgba(0.1,0.1,0.1,0.3)
        Row{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                text: "拾取/控制"
                onClicked: {
                    glMouse.clickTool(String(toolOne))
                }
            }
            Button{
                text: "选区"
                onClicked: {
                    glMouse.clickTool(String(toolTwo))
                }
            }
            CheckBox{
                height: parent.height
                text: "坐标轴"
                checked: true
                onClicked: glViewer.axesVisible(checked)
                style: CheckBoxStyle{
                    label: Label{
                        text:control.text
                        color: "white"
                    }
                }
            }
            CheckBox{
                height: parent.height
                text: "包围盒"
                checked: true
                onClicked: glViewer.boxVisible(checked)
                style: CheckBoxStyle{
                    label: Label{
                        text:control.text
                        color: "white"
                    }
                }
            }

            spacing: 10
        }
    }

    // 模型旋转、放缩、移动
    Connections{
        id: toolOne
        target: glMouse
        property point ppos: Qt.point(0,0)
        enabled: glMouse.current_tool === String(toolOne)

        onPressed: {
            ppos = Qt.point(mouse.x, mouse.y)
        }
        onDoubleClicked:{
            glViewer.singleFacePick(mouse.x*devicePixelRatio, mouse.y*devicePixelRatio)
        }

        onPositionChanged: {
            if(mouse.buttons & Qt.LeftButton)
                glViewer.rotate(mouse.x-ppos.x, mouse.y - ppos.y)
            else if(mouse.buttons & Qt.RightButton){
                glViewer.move(mouse.x-ppos.x, mouse.y - ppos.y)
            }
            ppos = Qt.point(mouse.x, mouse.y)
        }

        onWheel: {
            glViewer.scaleBy(-wheel.angleDelta.y*0.001)
        }
        Component.onCompleted: {
            glMouse.clickTool(String(this))
        }
    }

    // 选区工具
    Connections{
        id: toolTwo
        target: glMouse

        enabled: glMouse.current_tool === String(toolTwo)

        onPressed: {
            selectCanvas.moveTo(mouse.x, mouse.y)
        }

        onPositionChanged: {
           selectCanvas.lineTo(mouse.x, mouse.y)
        }

        onReleased: {
            glViewer.screenAreaPick(selectCanvas)
//            selectCanvas.select()
        }
    }
}
