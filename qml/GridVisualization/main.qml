import QtQuick 2.0
import Qt3D 2.0
import mbrdna.qml.components 1.0

Rectangle {
    color:"silver"
    width: 1440
    height: 540

    Viewport
    {
        id: viewport
        anchors.fill: parent
        blending:true
        navigation:true

        camera: Camera {
            id: cam
            eye: Qt.vector3d(0, 2, 10);
            fieldOfView: 4.55

            onEyeChanged: {
                console.log("Camera eye changed eye.xyz = [" + eye.x + "," + eye.y + "," + eye.z + "]");
            }

            onFieldOfViewChanged: {
                console.log("FOV Changed: " + Math.round(cam.fieldOfView * 100) / 100);
            }

        }

        GridEqualizer {
            id:grid
            rows: 32
            cols: 32
        }


    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        property int count: 0;
        onTriggered: {
           // grid.setTargetHeightsAtRandom();
            grid.setTargetHeightsBellCurve(count % 3 === 0);
            count++;
        }
    }

    BeforeRenderItem{
        onBeforeRender: {
            grid.advanceAnimation();
        }
    }

}
