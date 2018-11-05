import QtQuick 2.0
import QtQuick.Controls 1.4
import MeshDevelop 1.0
import QtQuick.Controls.Styles 1.4

Rectangle {
    color: '#909294'
    Text{
        anchors.fill: parent
        y: 100
        x: 20
        text: "meshDevelop"
    }

    ListView{
//       visible: false
       y: 100
       anchors.horizontalCenter: parent.horizontalCenter
       height: 300
       model: ListModel{
           id: body_components
           ListElement{name: "全身"; check:true}
           ListElement{name: "左腿"; check:true}
           ListElement{name: "右腿"; check:true}
           ListElement{name: "左手"; check:true}
           ListElement{name: "右手"; check:true}
           ListElement{name: "臀"; check:true}
           ListElement{name: "胸"; check:true}
           ListElement{name: "肩"; check:true}
           ListElement{name: "头"; check:true}
       }
       delegate:CheckBox{
           id: checkBox
           text: name
           checked: check
           style: CheckBoxStyle{
               label: Label{
                   text:control.text
                   color: "white"
               }
           }
           Binding{
               target: checkBox
               property: "checked"
               value: check
           }

           onClicked:  {
               if(model.index === 0) {
                    for(var i = 1; i < body_components.count; i++){
                        body_components.setProperty(i,"check",checked)
                        SubBackends.checkComponents(i-1, checked)
                    }
               }else {
                   body_components.setProperty(model.index,"check",checked)
                   SubBackends.checkComponents(model.index-1, checked)
               }
           }
       }
       spacing: 10
    }
}
