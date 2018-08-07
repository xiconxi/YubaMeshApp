import QtQuick 2.10
import QtQuick.Controls 1.4
import QtQuick.Window 2.10
import QtQuick.Controls.Styles 1.2
import QtQuick.XmlListModel 2.0
import GLViewer 1.0 as Ybgl
import YubaCore 1.0
YubaWindow {
    id: wind
    visible: true
    width: 1280
    height: 800
    flags: Qt.WindowTitleHint | Qt.Window | Qt.MSWindowsFixedSizeDialogHint | Qt.WindowSystemMenuHint  | Qt.WindowCloseButtonHint
    property var glViewer
    title: qsTr("[Yuba's app]")

    Ybgl.Viewer{
        anchors.left: leftToolView.right
        anchors.top: toolCategory.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Shortcut{
            sequence: "Ctrl+G"
            onActivated: {
                glViewer.grabToImage(function(image){
                    image.saveToFile(yubaDoc+Qt.formatDateTime(new Date(), "yyyyMMdd-hhmmss")+".png")
                })
            }
        }

        DropArea{
            id: dropArea
            anchors.fill: parent
            onDropped: {
                if(drop.hasUrls && drop.urls.length === 1) {
                    modelDroped(drop.urls[0])
                }else
                    drop.accept(true)
            }
            signal modelDroped(string fileUrl)
        }
    }



    Rectangle{
        id:toolCategory
        height: 20
        color: '#615d5c'
        width: parent.width
        ListView{
            id:toolCategoryList
            anchors.fill: parent
            orientation: Qt.Horizontal
            property int categoryId: 0
            boundsBehavior: Flickable.StopAtBounds
            spacing: 8
            model:XmlListModel {
                id: menuXmlModel
                source: "file:///"+appPath+"/pluginsConfig.xml"
                query: "/channel/plugin"
                XmlRole { name: "name"; query: "name/string()" }
                XmlRole { name: "module"; query: "module/string()" }
                XmlRole { name: "version"; query: "version/string()" }

                onStatusChanged: {
//                    by using this, will open the last menu(currentIndex will change twice!!!!!)
//                    if(status== XmlListModel.Ready){
//                        toolCategoryList.currentIndex = count -1
//                    }
                }
            }

            onCurrentIndexChanged:{
                if( leftToolView.subView)
                    leftToolView.subView.destroy();
                var M = menuXmlModel.get(currentIndex)
                glViewer.registerModule(M.module)

                var plugin_str ='import QtQuick 2.0; \nimport '+M.module + ' ' +
                                M.version+'; \n'+'Tools{anchors.fill :parent\n
                                Component.onCompleted: SubBackends.construction()\n' +
                                "Component.onDestruction: {glViewer.unregisterModule('"+M.module+"')\n
                                SubBackends.destruction()}
            Connections{
                target: dropArea
                onModelDroped: {
console.log(SubBackends.openFile(fileUrl))
}
            }

                                }";
                leftToolView.subView = Qt.createQmlObject(plugin_str,leftToolView, "dynamicPluginHub");
            }

            delegate: Button {
                width: 60; height: 20
                text: name
                id: categoryButton

                style: ButtonStyle {
                    background: Rectangle {
                        id: background
                        border.width: control.activeFocus ? 5 : 0
                        gradient: categoryButton.ListView.isCurrentItem ? Gradients.gd1:(categoryButton.hovered?Gradients.gd2:Gradients.gd3)
                    }
                    label: Text {
                        text: control.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:   Text.AlignVCenter
                    }

                }
                onClicked: {
                    toolCategoryList.currentIndex = model.index
                }
            }
        }
    }

    Rectangle{
        id: leftToolView
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        width: 180
        color: '#909294'
        property var subView
    }

}
