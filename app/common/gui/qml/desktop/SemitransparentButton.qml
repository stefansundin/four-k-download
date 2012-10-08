import QtQuick 1.1
import Desktop 0.1
import "custom" as Components

Components.Button {
    id:button

    property alias containsMouse: tooltip.containsMouse
    property string styleHint
    property bool highlightTextColor: false
    property bool textBold: false
    property real normalOpacity: 0.25
    property real hoverOpacity: 1.0
    property real pressedOpacity: 1.0

    implicitWidth: backgroundItem.implicitWidth
    implicitHeight: backgroundItem.implicitHeight

    TooltipArea {
        // Note this will eat hover events
        id: tooltip
        anchors.fill: parent
        text: button.tooltip
    }

    SystemPalette {id: systemPallete}

    background: StyleItem {
        id: styleitem
        anchors.fill: parent
        elementType: "toolbutton"
        on: pressed | checked
        sunken: pressed
        raised: true
        hover: true
        info: __position
        hint: button.styleHint
        opacity: {
            if (pressed)
                return pressedOpacity
            else if (containsMouse)
                return hoverOpacity
            else
                return normalOpacity
        }
    }

    Image {
        id: iconitem
        smooth: true
        fillMode: Image.PreserveAspectFit
        source: button.iconSource
    }

    Text {
        id: textitem
        font.bold: button.textBold
        color: button.highlightTextColor ? systemPallete.highlightedText : systemPallete.text
        text: button.text
    }

    state: {
        if (button.iconSource == "")
            return "onlytext"

        if (button.text == "")
            return "onlyicon"

        return "horizontal";
    }

    states: [
        State {
            name: "horizontal"

            AnchorChanges {
                target: iconitem
                anchors.verticalCenter: button.verticalCenter
                anchors.left: button.left
            }
            PropertyChanges {
                target: iconitem
                anchors.margins: 5
            }
            AnchorChanges {
                target: textitem
                anchors.verticalCenter: button.verticalCenter
                anchors.right: button.right
            }
            PropertyChanges {
                target: textitem
                anchors.margins: 8
            }
            PropertyChanges {
                target: button
                implicitWidth: textitem.paintedWidth + iconitem.paintedWidth + 8 + 8
                implicitHeight: Math.max(textitem.paintedHeight, iconitem.paintedHeight) + 4 + 5
            }
        },

        State {
            name: "onlyicon"

            AnchorChanges {
                target: iconitem
                anchors.verticalCenter: button.verticalCenter
                anchors.horizontalCenter: button.horizontalCenter
            }
            PropertyChanges {
                target: textitem
                visible: false
            }
            PropertyChanges {
                target: button
                implicitWidth: iconitem.paintedWidth + 2 + 1
                implicitHeight: iconitem.paintedHeight + 2 + 2
            }
        },

        State {
            name: "onlytext"

            PropertyChanges {
                target: iconitem
                visible: false
            }
            AnchorChanges {
                target: textitem
                anchors.verticalCenter: button.verticalCenter
                anchors.horizontalCenter: button.horizontalCenter
            }
            PropertyChanges {
                target: button
                implicitWidth: textitem.paintedWidth + 4 + 8
                implicitHeight: textitem.paintedHeight + 4 + 5
            }
        }
    ]
}
