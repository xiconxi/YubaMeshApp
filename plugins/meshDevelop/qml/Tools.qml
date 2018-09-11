import QtQuick 2.0
import QtQuick.Controls 1.4
import MeshDevelop 1.0
Item {
    Text{
        anchors.fill: parent
        y: 100
        x: 20
        text: "meshDevelop"
    }

    Column{
        anchors.horizontalCenter: parent.horizontalCenter
        y: 100
        Button{
            text: "截图"
            onClicked: {
                glViewer.grabToImage(function(image){
                    image.saveToFile(yubaDoc+Qt.formatDateTime(new Date(), "yyyyMMdd-hhmmss")+".png")
                })
            }
        }

        Button{
            text: "关闭特效"
            onClicked: SubBackends.draw_for("mesh")
        }

        spacing: 10
    }
    Connections{
        target: GMouse
        onClicked:{
//            console.log("meshDevelop: ","glViewe click")
        }
    }

}
