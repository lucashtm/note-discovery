import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import app.controllers 1.0

Window {
    width: 640
    height: 280
    visible: true
    title: qsTr('Note Discovery')
    minimumHeight: 280
    maximumHeight: 280
    minimumWidth: 640
    maximumWidth: 640

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 50
        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            Text {
                id: note
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 36
                text: 'Note: ' + qsTr(StartupController.note);
                color: "#333"
            }

            Text {
                id: frequency
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 24
                text: 'Frequency: ' + qsTr(StartupController.frequency) + 'Hz';
                color: "#c1c1c1"
            }
        }


//        Button {
//            anchors.horizontalCenter: parent.horizontalCenter
//            text: "Abrir Arquivo"
//            onClicked: fileDialog.open()
//        }

        RecordButton {
            onClicked: StartupController.startRecording()
        }

        ComboBox {
            anchors.horizontalCenter: parent.horizontalCenter
            textRole: 'text'
            valueRole: 'value'
            currentIndex: StartupController.selectedInputDeviceIndex
            width: 300
            model: {
                let i = 0;
                StartupController.inputDevices.map(device => {
                    return { 'text': device, 'value':  i++ };
                });
            }
            onActivated: StartupController.setSelectedInputDevice(index)
        }
    }

    FileDialog {
        id: fileDialog
        onAccepted: {
            StartupController.read(this.fileUrl);
        }
    }
}
