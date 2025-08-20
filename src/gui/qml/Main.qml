import QtQuick

Window {
    width: 960
    height: 600
    minimumWidth: 400
    minimumHeight: 90
    visible: true
    title: "RepetitionAI"
    Rectangle {
        anchors.fill: parent
        color: "#121212"
    }
    Text {
        anchors.centerIn: parent
        text: "Hello"
        color: "white"
        font.pixelSize: 28
    }
}
