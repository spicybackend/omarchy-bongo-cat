import QtQuick
import Quickshell.Io
import qs.Ui

BarWidget {
    id: root
    moduleName: "jordane.bongo-cat"

    readonly property string configuredDevice: settings && settings.device ? String(settings.device) : ""
    readonly property string listenerPath: String(Qt.resolvedUrl("keyboard-listener")).replace(/^file:\/\//, "")
    readonly property string statusMessage: listenerOnline
        ? "Bongo Cat: listening for key presses"
        : (listenerError || "Bongo Cat: keyboard listener is unavailable")

    property bool listenerOnline: false
    property bool leftKeyPressed: false
    property bool rightKeyPressed: false
    property string listenerError: ""

    implicitWidth: vertical ? barSize : 54
    implicitHeight: barSize

    function setPaw(side, isDown) {
        if (side === "left") leftKeyPressed = isDown
        else rightKeyPressed = isDown
    }

    function handleListenerLine(line) {
        const value = String(line || "").trim()
        if (value === "ready") {
            listenerOnline = true
            listenerError = ""
        } else if (value === "left down" || value === "left up"
                   || value === "right down" || value === "right up") {
            const parts = value.split(" ")
            setPaw(parts[0], parts[1] === "down")
        }
    }

    Image {
        id: cat
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        source: root.leftKeyPressed && root.rightKeyPressed
            ? "assets/cat-both.png"
            : (root.leftKeyPressed ? "assets/cat-right.png"
               : (root.rightKeyPressed ? "assets/cat-left.png" : "assets/cat-rest.png"))
        fillMode: Image.PreserveAspectFit
        opacity: root.listenerOnline ? 1 : 0.45
        smooth: true
    }


    Timer {
        id: retryTimer
        interval: 15000
        onTriggered: if (!listener.running) listener.running = true
    }

    Process {
        id: listener
        command: root.configuredDevice === ""
            ? [root.listenerPath]
            : [root.listenerPath, root.configuredDevice]
        stdout: SplitParser {
            onRead: function(line) { root.handleListenerLine(line) }
        }
        stderr: SplitParser {
            onRead: function(line) {
                const message = String(line || "").trim()
                if (message !== "") root.listenerError = message
            }
        }
        onExited: {
            root.listenerOnline = false
            root.leftKeyPressed = false
            root.rightKeyPressed = false
            retryTimer.restart()
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: if (root.bar) root.bar.showTooltip(root, root.statusMessage)
        onExited: if (root.bar) root.bar.hideTooltip(root)
    }

    Component.onCompleted: listener.running = true
}
