// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import QtMobility.sensors 1.2

Image {
    id: icon
    width: 64
    height: 64
    state: "landscape"

    property alias text: textElement.text

    signal clicked()

    states: [
        State {
            name: "portrait"
            PropertyChanges {
                target: icon
                rotation: -90
            }
        },
        State {
            name: "landscape"
            PropertyChanges {
                target: icon
                rotation: 0
            }
        }
    ]

    transitions: [
        Transition {
            ParallelAnimation {
                NumberAnimation { properties: "rotation"; duration: 500; easing.type: Easing.InOutQuad }
            }
        }
    ]

    OrientationSensor {
        id: sensor
        active: true
        onReadingChanged: {
            if (reading.orientation == OrientationReading.RightUp)
                icon.state = "landscape"
            else if (reading.orientation == OrientationReading.TopUp)
                icon.state = "portrait"
        }
    }

    Image {
        id: shadow
        anchors.fill: parent
        source: _ICON_LOCATION + "icon-m-common-camera.png"
        opacity: 0.2
        visible: false
    }

    MouseArea {
        anchors.fill: parent

        onClicked: parent.clicked()
        onPressed: shadow.visible = true;
        onReleased: shadow.visible = false;
    }

    Text {
        id: textElement
        anchors.centerIn: parent
        text: parent.text
    }
}

