import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: settingsPage
    //orientationLock: PageOrientation.LockLandscape
    tools: ToolBarLayout {
        id: commonTools

        visible: false
        ToolIcon { platformIconId: "toolbar-back";
            anchors.left: parent.left
            onClicked: {
                pageStack.pop()
            }
        }
    }

    Flickable {
        anchors.fill: parent
        contentHeight: 1000

        Label {
            id: labelProgram
            anchors.top: parent.top
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.leftMargin: 16
            font.pixelSize: _SMALL_FONT_SIZE
            color: "grey"
            text: "Program"
        }
        Separator {
            anchors.left: labelProgram.right
            anchors.leftMargin: 16
            anchors.verticalCenter: labelProgram.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 16
        }

        ButtonRow {
            id: programButtonRow
            anchors.top: labelProgram.bottom
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            checkedButton: videoDVD
            //platformStyle: StyledButton {}
            Button { text: "1"
                id: videoVGA
               // onClicked: settingsObject.setVideoResolution(0)
            }
            Button { text: "2"
                id: videoDVD
                //onClicked: settingsObject.setVideoResolution(1)
            }
            Button { text: "3"
                id: videoHD
               // onClicked: settingsObject.setVideoResolution(2)
            }
            Button { text: "4"
          //      id: videoHD
               // onClicked: settingsObject.setVideoResolution(2)
            }
            Button { text: "5"
             //   id: videoHD
               // onClicked: settingsObject.setVideoResolution(2)
            }
            Button { text: "6"
              //  id: videoHD
               // onClicked: settingsObject.setVideoResolution(2)
            }
        }

        Label {
            id: labelLight
            anchors.top: programButtonRow.bottom
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.leftMargin: 16
            font.pixelSize: _SMALL_FONT_SIZE
            color: "grey"
            text: "Light"
        }
        Separator {
            anchors.left: labelLight.right
            anchors.leftMargin: 16
            anchors.verticalCenter: labelLight.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 16
        }

        ButtonRow {
            id: lightButtonRow
            anchors.top: labelLight.bottom
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            //platformStyle: StyledButton {}
            Button { text: "1"
               // onClicked: settingsObject.setVideoResolution(0)
            }
            Button { text: "2"
                //onClicked: settingsObject.setVideoResolution(1)
            }
            Button { text: "3"
               // onClicked: settingsObject.setVideoResolution(2)
            }
            Button { text: "4"
          //      id: videoHD
               // onClicked: settingsObject.setVideoResolution(2)
            }
        }

        Label {
            id: labelResolution
            anchors.top: lightButtonRow.bottom
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.leftMargin: 16
            font.pixelSize: _SMALL_FONT_SIZE
            color: "grey"
            text: "Resolution"
        }
        Separator {
            anchors.left: labelResolution.right
            anchors.leftMargin: 16
            anchors.verticalCenter: labelResolution.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 16
        }

        ButtonRow {
            id: resolutionButtonRow
            anchors.top: labelResolution.bottom
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            //platformStyle: StyledButton {}
            Button { text: "1"
               // onClicked: settingsObject.setVideoResolution(0)
            }
            Button { text: "2"
                //onClicked: settingsObject.setVideoResolution(1)
            }
            Button { text: "3"
               // onClicked: settingsObject.setVideoResolution(2)
            }
            Button { text: "4"
          //      id: videoHD
               // onClicked: settingsObject.setVideoResolution(2)
            }
            Button { text: "5"
          //      id: videoHD
               // onClicked: settingsObject.setVideoResolution(2)
            }
        }

        Label {
            id: labelFaceDetection
            anchors.top: resolutionButtonRow.bottom
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.leftMargin: 16
            font.pixelSize: _STANDARD_FONT_SIZE
            text: "Face detection: "
        }

        Switch {
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.verticalCenter: labelFaceDetection.verticalCenter
            enabled: false
        }

    }

}
