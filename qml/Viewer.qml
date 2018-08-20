import QtQuick 2.0
import GLViewer 1.0
import "."
Item {
    GlViewer{
        id: glViewer
        anchors.fill: parent
        Component.onCompleted:
            wind.glViewer = glViewer
        Text{
            anchors.left: parent.left
            anchors.top: parent.top
            text: "FPS: " + glViewer.fps
            color: "red"
        }
    }

    MouseArea{
        objectName: "GLMouseAreaObject"
        id: glMouse
        anchors.fill: glViewer
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        property string global_tool: ""
        property string current_tool: ""
        property string previs_tool: ""

        function clickTool(hash){
            if(global_tool === ""){
                global_tool = hash
                previs_tool = hash
                current_tool = hash
            }else{
                // 取消工具
                if(hash === current_tool && previs_tool !== ""){
                    current_tool = previs_tool
                    previs_tool = global_tool
                }else {
                    previs_tool = current_tool
                    current_tool = hash
                }

            }
        }
    }

    Tools{
        anchors.fill: glViewer
    }
}
