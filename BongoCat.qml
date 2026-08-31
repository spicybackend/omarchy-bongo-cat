import QtQuick
import Quickshell.Io
import qs.Ui
import qs.Commons

BarWidget {
    id: root
    moduleName: "spicybackend.bongo-cat"

    readonly property string configuredDevice: settings && settings.device ? String(settings.device) : ""
    readonly property string listenerPath: String(Qt.resolvedUrl("keyboard-listener")).replace(/^file:\/\//, "")
    readonly property bool showWpm: displayMode !== 1
    readonly property bool showCat: displayMode !== 2
    readonly property int wpmWindowMilliseconds: 10000
    readonly property string displayModeName: displayMode === 0 ? "Bongo Cat + WPM"
        : (displayMode === 1 ? "Bongo Cat" : "WPM")
    readonly property bool lightTheme: Color.background.r * 0.2126
        + Color.background.g * 0.7152 + Color.background.b * 0.0722 > 0.5
    readonly property string statusMessage: listenerOnline
        ? "Bongo Cat: " + displayModeName + " · " + wordsPerMinute
          + " WPM (rolling 10 s; left-click: display)"
        : (listenerError || "Bongo Cat: keyboard listener is unavailable")

    property bool listenerOnline: false
    property bool leftKeyPressed: false
    property bool rightKeyPressed: false
    function normalizedDisplayMode(value) {
        const mode = Math.floor(Number(value))
        return mode >= 0 && mode <= 2 ? mode : 0
    }

    property int displayMode: normalizedDisplayMode(setting("displayMode", 0))
    property int wordsPerMinute: 0
    property var typingTimestamps: []
    property string listenerError: ""
    readonly property real leftBarPadding: Style.spaceReal(6)
    readonly property real rightBarPadding: Style.spaceReal(3)

    implicitWidth: content.implicitWidth + leftBarPadding + rightBarPadding
    implicitHeight: Math.max(content.implicitHeight, barSize)

    function setPaw(side, isDown) {
        if (side === "left") leftKeyPressed = isDown
        else rightKeyPressed = isDown
    }

    function refreshWpm() {
        const cutoff = Date.now() - wpmWindowMilliseconds
        typingTimestamps = typingTimestamps.filter(function(timestamp) { return timestamp >= cutoff })
        wordsPerMinute = Math.round(typingTimestamps.length * 60000 / (wpmWindowMilliseconds * 5))
    }

    function recordTypingKey() {
        typingTimestamps = typingTimestamps.concat([Date.now()])
        refreshWpm()
    }

    function cycleDisplayMode() {
        const next = (displayMode + 1) % 3
        displayMode = next
        settings = Object.assign({}, settings || {}, { displayMode: next })
        if (bar && bar.shell && typeof bar.shell.updateEntryInline === "function")
            bar.shell.updateEntryInline(moduleName, settings)
        if (bar) bar.showTooltip(root, statusMessage)
    }

    function catAsset(frame) {
        return "assets/" + (lightTheme ? "black-cat-" : "cat-") + frame + ".png"
    }

    function handleListenerLine(line) {
        const value = String(line || "").trim()
        if (value === "ready") {
            listenerOnline = true
            listenerError = ""
        } else if (value === "typed") {
            recordTypingKey()
        } else if (value === "left down" || value === "left up"
                   || value === "right down" || value === "right up") {
            const parts = value.split(" ")
            setPaw(parts[0], parts[1] === "down")
        }
    }

    Row {
        id: content
        anchors.left: parent.left
        anchors.leftMargin: root.leftBarPadding
        anchors.verticalCenter: parent.verticalCenter
        spacing: Style.space(5)

        Text {
            visible: root.showWpm
            height: root.barSize
            verticalAlignment: Text.AlignVCenter
            text: root.wordsPerMinute + " WPM"
            color: root.bar ? root.bar.foreground : "white"
            font.family: root.bar ? root.bar.fontFamily : "monospace"
            font.pixelSize: Style.font.bodySmall
        }

        Item {
            visible: root.showCat
            width: root.vertical ? Math.max(18, root.barSize - 2) : 48
            height: root.barSize

            Image {
                anchors.centerIn: parent
                width: parent.width
                height: Math.max(18, root.barSize - 2)
                source: root.leftKeyPressed && root.rightKeyPressed
                    ? root.catAsset("both")
                    : (root.leftKeyPressed ? root.catAsset("right")
                       : (root.rightKeyPressed ? root.catAsset("left") : root.catAsset("rest")))
                fillMode: Image.PreserveAspectFit
                opacity: root.listenerOnline ? 1 : 0.45
                smooth: true
            }
        }
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: root.refreshWpm()
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
        onClicked: root.cycleDisplayMode()
        onEntered: if (root.bar) root.bar.showTooltip(root, root.statusMessage)
        onExited: if (root.bar) root.bar.hideTooltip(root)
    }

    Component.onCompleted: listener.running = true
}
