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
        anchors.top: toolPanel.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right

    }

    Rectangle{
        id:toolCategory
        height: 18
        color: '#615d5c'
        width: parent.width
        ListView{
            id:toolCategoryList
            anchors.fill: parent
            orientation: Qt.Horizontal
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
//                    using this, will open the last menu(currentIndex will change twice!!!!!)
//                    if(status== XmlListModel.Ready){
//                        toolCategoryList.currentIndex = count -1
//                    }
                }
            }

            property string module
            property string version

            onCurrentIndexChanged:{
                module = menuXmlModel.get(currentIndex).module
                version = menuXmlModel.get(currentIndex).version
                toolXmlModel.source = "file:///" + appPath + module + "/ToolLists.xml"

//                if( leftToolView.subView){
//                    leftToolView.subView.destroy();
//                    leftToolView.subView = null;
//                }
//                var M = menuXmlModel.get(currentIndex)
//                var plugin_str ='import QtQuick 2.0; \nimport '+M.module + ' ' + M.version+'; \n'+
//                                'Tools{anchors.fill :parent\n
//                                Component.onCompleted: SubBackends.construction()\n' +
//                                "Connections{ target: dropArea; onModelDroped: SubBackends.importMesh(fileUrl) }
//                                }";
//                leftToolView.subView = Qt.createQmlObject(plugin_str,leftToolView, "dynamicPluginHub");

            }

            delegate: Button {
                width: 60; height: toolCategory.height
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
        id: toolPanel
        height: 60
        color: '#808080'
        width: parent.width
        anchors.top: toolCategory.bottom
        ListView{
            id: toolPanelList
            anchors.fill: parent
            orientation: Qt.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            currentIndex: -1
            spacing: 8
            property Component toolComponent
            model:XmlListModel {
                id: toolXmlModel
                query: "/channel/tool"
                XmlRole { name: "name"; query: "name/string()" }
                XmlRole { name: "source"; query: "source/string()" }
                onStatusChanged: {
                    if(status== XmlListModel.Ready)
                        leftToolView.source = "file:///" + appPath + toolCategoryList.module + "/"+"ToolX.qml"
                }
            }

            onCurrentIndexChanged:{
                leftToolView.source = "file:///" + appPath + toolCategoryList.module + "/" + toolXmlModel.get(currentIndex).source
            }

            delegate: Button {
                width: toolPanel.height; height: toolPanel.height
                text: name
                id: toolButton

                style: ButtonStyle {
                    background: Rectangle {
                        border.width: control.activeFocus ? 10 : 5
                        gradient: toolButton.ListView.isCurrentItem ? Gradients.gd2:(toolButton.hovered?Gradients.gd1:Gradients.gd5)
                    }
                    label: Text {
                        text: control.text
                        color: "black"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:   Text.AlignVCenter
                    }

                }
                onClicked: {
                    toolPanelList.currentIndex = model.index
                }
            }
        }
    }

    Loader{
        id: leftToolView
        anchors.left: parent.left
        anchors.top: toolPanel.bottom
        anchors.bottom: parent.bottom
        width: 200
    }
}
