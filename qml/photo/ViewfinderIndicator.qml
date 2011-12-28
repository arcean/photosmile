// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: indicator
    property string color: "yellow"
    property int clicked_width: 160
    property int clicked_height: 100
    property int clicked_overlap_width: 0
    property int clicked_overlap_height: 0
    property bool reverse: true

    width: 160
    height: 100
    opacity: 0.5
    state: "normal"

    states: [
        State {
            name: "normal";
            PropertyChanges {
                target: indicator;
                clicked_width: 160
                clicked_height: 100
                clicked_overlap_width: 0
                clicked_overlap_height: 0
            }
            PropertyChanges {
                target: stateTimer
                running: false
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: indicator;
                clicked_width: 80
                clicked_height: 50
                clicked_overlap_width: 40
                clicked_overlap_height: 25
            }
            PropertyChanges {
                target: stateTimer
                running: true
            }
        }
    ]

    transitions: [
        Transition {
            ParallelAnimation {
                NumberAnimation { properties: "clicked_overlap_width, clicked_overlap_height, clicked_width, clicked_height"; duration: 500; easing.type: Easing.InOutQuad }
            }
        }
    ]

    Timer {
        id: stateTimer
        running: false
        repeat: true
        interval: 500
        onTriggered: {
            if(reverse)
                state = "normal"
        }
    }


    Rectangle {
        id: upperLeft
        x: clicked_overlap_width
        y: clicked_overlap_height
        width: clicked_width / 4
        height: 4
        //color: "grey"
        color: parent.color
    }

    Rectangle {
        id: upperRight
        x: clicked_width - (clicked_width / 4) + clicked_overlap_width
        y: clicked_overlap_height
        width: clicked_width / 4
        height: 4
        //color: "grey"
        color: parent.color
    }

    Rectangle {
        id: bottomLeft
        x: clicked_overlap_width
        y: clicked_height - height + clicked_overlap_height
        width: clicked_width / 4
        height: 4
        //color: "grey"
        color: parent.color
    }

    Rectangle {
        id: bottomRight
        x: clicked_width - (clicked_width / 4) + clicked_overlap_width
        y: clicked_height - height + clicked_overlap_height
        width: clicked_width / 4
        height: 4
        //color: "grey"
        color: parent.color
    }

    Rectangle {
        id: leftUpper
        x: clicked_overlap_width
        y: 4 + clicked_overlap_height
        width: 4
        height: clicked_height / 4
        //color: "grey"
        color: parent.color
    }

    Rectangle {
        id: leftBottom
        x: clicked_overlap_width
        y: clicked_height - (clicked_height / 4) - 4 + clicked_overlap_height
        width: 4
        height: clicked_height / 4
        //color: "grey"
        color: parent.color
    }

    Rectangle {
        id: rightUpper
        x: clicked_width - 4 + clicked_overlap_width
        y: 4 + clicked_overlap_height
        width: 4
        height: clicked_height / 4
        //color: "grey"
        color: parent.color
    }

    Rectangle {
        id: rightBottom
        x: clicked_width - 4 + clicked_overlap_width
        y: clicked_height - (clicked_height / 4) - 4 + clicked_overlap_height
        width: 4
        height: clicked_height / 4
        //color: "grey"
        color: parent.color
    }
}
