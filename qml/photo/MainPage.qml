import QtQuick 1.1
import com.nokia.meego 1.0
import Camera 1.0
import QtMobility.sensors 1.2

Page {
    id: viewfinderPage
    orientationLock: PageOrientation.LockLandscape
    state: "show"

    property int toggle: 0
    property int orientation: 0
    property bool camera_ready: false
    property int m_width: 0
    property int m_height: 0
    property int indi_x: 0
    property int indi_y: 0
    property bool obtainedViewfinderSize: false

    Component.onCompleted: {
        camMirrorScale.xScale = -1 * camMirrorScale.xScale;
        frontCam.launchMainCamera();
        frontCam2.launchFrontCamera();
        frontCam.start();
        camera_ready = true;
      //  mpxButton.text = (frontCam.visible ? frontCam.getMPix() : frontCam2.getMPix()) + " MP";
        //frontCam2.start()
    }
/*
    OrientationSensor {
        id:orientationSensor
        dataRate: 200
        active:platformWindow.visible
        onReadingChanged: {
            viewfinderPage.orientation = reading.orientation
            updateScaleRotation()
        }
    }

    function updateScaleRotation()
    {
        if(camMirrorScale.xScale>0 &&
                (viewfinderPage.orientation == 1 ||
                 viewfinderPage.orientation == 2))
        {
            camMirrorRotate.angle = 180
        }
        else {
            camMirrorRotate.angle = 0
        }
    }*/

    function brick()
    {
        if(toggle == 0) {
            frontCam.unload()
            if(rot.angle == 180)
                rot.angle = 0
            else
                rot.angle = 180
        }
        else {
            frontCam2.unload()
            if(rot.angle == 180)
                rot.angle = 0
            else
                rot.angle = 180
        }
        if(!flipTimer.running)
            flipTimer.start()
    }

    function flipCamera()
    {
        if(toggle == 0) {
            frontCam2.visible = true
            frontCam2.start()
            toggle = 1
        }
        else {
            frontCam.visible = true
            frontCam.start()
            toggle = 0
        }
        camera_ready = true
    }

    function takeFocus()
    {
        if(frontCam.visible)
            frontCam.takeFocus();
        else
            frontCam2.takeFocus();
    }

    function takePhoto()
    {
        if(frontCam.visible)
            frontCam.takePhoto();
        else
            frontCam2.takePhoto();
    }

    function getIndicatorX()
    {
        indi_x = indi.x + (indi.width / 2);
        indi_x -= m_width;
        console.log('indi_x: ' + indi_x);
    }

    function getIndicatorY()
    {
        indi_y = indi.y + (indi.height / 2);
        indi_y -= m_height;
        console.log('indi_y: ' + indi_y);
    }

    function setRegion(width, height)
    {
        console.log('x:' + x + ' y:' + y);

        m_width = (854 - width) / 2;
        if(m_width < 0)
            m_width = 0;

        m_height = (480 - height) / 2;
        if(m_height < 0)
            m_height = 0;
    }

    function setIndicatorPosition(mouseX, mouseY)
    {
        indi.x = mouseX - (indi.width / 2);
        if (indi.x < m_width)
            indi.x = m_width;
        if (indi.x + indi.width > parent.width - m_width)
            indi.x = parent.width - indi.width - m_width;

        indi.y = mouseY - (indi.height / 2);
        if (indi.y < m_height)
            indi.y = m_height;
        if (indi.y + indi.height > parent.height - m_height)
            indi.y = parent.height - indi.height - m_height;

        if (mouseX >= 360 && mouseX <= 494) {
            if(mouseY >= 200 && mouseY <= 280) {
                indi.x = (parent.width / 2) - (indi.width / 2)
                indi.y = (parent.height / 2) - (indi.height / 2)
                if(frontCam.visible)
                    frontCam.setPointModeFocusToFaceDetection();
                else
                    frontCam2.setPointModeFocusToFaceDetection();
            }
        }
        else {
            if(frontCam.visible)
                frontCam.setPointModeFocusToCustom();
            else
                frontCam2.setPointModeFocusToCustom();
        }
    }

    Timer {
        id: flipTimer
        interval: 510
        running: false
        repeat: false
        onTriggered: flipCamera()
    }

    Scale {
        id: camMirrorScale
        origin.x: parent.width/2
        origin.y: parent.height/2
        xScale: 1
    }

    Rotation {
        id: camMirrorRotate
        origin.x: parent.width/2
        origin.y: parent.height/2
        angle: 0
    }

    Connections {
        target: platformWindow

        onActiveChanged: {
            if(platformWindow.active) {
               // frontCam.start();
            }
            else {
               // frontCam.stop();
            }
        }
    }
/*
    Rectangle {
        id: frontCam
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        color: "white"
        opacity: 0.5

        transform: Rotation {
            id: rot
            origin.x: frontCam.width / 2;
            origin.y: frontCam.height / 2;
            axis.x: 1; axis.y: 0; axis.z: 0
            angle: 0

            Behavior on angle { PropertyAnimation{} }
        }
    }*/

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Rectangle {
        id: flipable
        anchors.fill: parent
        color: "black"
        transform: Rotation {
            id: rot
            origin.x: width / 2;
            origin.y: height / 2;
            axis.x: 1; axis.y: 0; axis.z: 0
            angle: 0

            Behavior on angle { PropertyAnimation{duration: 500} }
        }
    }

    Camera {
        id:frontCam
        x: 0
        y: 0
        width: parent.width
        height: parent.height
       // transform: [camMirrorScale, camMirrorRotate]
      //  aspectRatio: Qt.KeepAspectRatioByExpanding
        //visible: false
        onMpixChanged: mpxButton.text = mpix + " MP";
        //onSignalFocusGained: ViewfinderIndicator.color
    }

    Camera {
        id:frontCam2
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        transform: [camMirrorScale, camMirrorRotate]
       // aspectRatio: Qt.KeepAspectRatioByExpanding
        visible: false
        onMpixChanged: mpxButton.text = mpix + " MP";
    }
/*
    MouseArea {
        anchors.fill: parent
        onPressed: {
         //   indi.reverse = false;
            mainPage.state = "hide"
            if (!obtainedViewfinderSize) {
                if (frontCam.visible)
                    setRegion(frontCam.getViewfinderNativeWidth(), frontCam.getViewfinderNativeHeight());
                else
                    setRegion(frontCam2.getViewfinderNativeWidth(), frontCam2.getViewfinderNativeHeight());
                obtainedViewfinderSize = true;
            }

            indi.state = "clicked";
            setIndicatorPosition(mouseX, mouseY);
        }
        onReleased: {
            //indi.state = "normal"
            indi.reverse = true;
            getIndicatorX();
            getIndicatorY();
            if(frontCam.visible)
                frontCam.setFocusPoint(indi_x, indi_y)
            else
                frontCam2.setFocusPoint(indi_x, indi_y)
            mainPage.state = "show"
            takeFocus();
        }

        onPositionChanged: {
            indi.reverse = false;
            indi.state = "clicked";
            setIndicatorPosition(mouseX, mouseY);
        }

      /*  onClicked: {
            if(indi.state == "normal")
                indi.state = "clicked"
            else
                indi.state = "normal"
        }*/
//    }

    ViewfinderIndicator {
        id: indi
        //anchors.centerIn: parent
        x: (parent.width / 2) - (width / 2)
        y: (parent.height / 2) - (height / 2)
    }

    UIButton {
        id: button
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.left: parent.left
        anchors.leftMargin: 24
        source: _ICON_LOCATION + "icon-m-common-camera.png"
        onClicked: {
            if(camera_ready) {
                camera_ready = false
                frontCam.visible = false
                frontCam2.visible = false
                brick()
            }
        }
    }

    UIButton {
        id: settings
        x: parent.width - 24 - width
        y: (parent.height / 2) - height
        source: _ICON_LOCATION + "icon-m-camera-scene-auto.png"
        onClicked: {
            pageStack.push(settingsPage);
        }
    }

    UIButtonWithText {
        id: mpxButton
        anchors.top: button.bottom
        anchors.topMargin: 24
        anchors.left: parent.left
        anchors.leftMargin: 24
        source: _ICON_LOCATION + "icon-m-common-presence-online.png"
        text: ""
        onClicked: {
            mpxButton.text = (frontCam.visible ? frontCam.getMPix() : frontCam2.getMPix()) + " MP";
            takeFocus();
        }
    }

    UIButtonWithText {
        id: takePhotoButton
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.right: parent.right
        anchors.rightMargin: 24
        source: _ICON_LOCATION + "icon-m-camera-shutter.png"
        text: ""
        onClicked: takePhoto();
    }

    states: [
        State {
            name: "show"
            PropertyChanges {
                target: settings
                x: parent.width - 24 - width
                y: (parent.height / 2) - height
            }
        },
        State {
            name: "hide"
            PropertyChanges {
                target: settings
                x: parent.width + 24
            }
        }
    ]

    transitions: [
        Transition {
            ParallelAnimation {
                NumberAnimation { properties: "x,y"; duration: 500; easing.type: Easing.InOutQuad }
            }
        }
    ]
}
