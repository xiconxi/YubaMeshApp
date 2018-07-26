import QtQuick 2.0

Item {
    Text{
        anchors.fill: parent
        y: 100
        x: 20
        text: "meshMorph"
    }
    Connections{
        target: GMouse
        onClicked:{
            console.log("meshMorph: ","glViewe click")
        }
    }
}
