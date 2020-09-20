import QtQuick 2.0
import QtQuick.Controls 2.12

RoundButton {
    anchors.horizontalCenter: parent.horizontalCenter
    id: recordButton
    background: Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 50
        height: 50
        radius: width*0.5
        border {
            color: '#310404'
            width: 2
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "#ce2424"
            width: 20
            height: 20
            radius: width*0.5
        }
    }
}
