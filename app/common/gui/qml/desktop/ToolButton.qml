import QtQuick 1.1
import Desktop 0.1
import "custom" as Components

Components.Button {
    id:button

    property alias containsMouse: tooltip.containsMouse
    property string iconName
    property string styleHint
    property int iconSize: (backgroundItem && backgroundItem.style === "mac" && button.styleHint.indexOf("segmented") !== -1) ? 16 : 24

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
        raised: containsMouse
        hover: containsMouse
        info: __position
        hint: button.styleHint

        Image {
            id: iconitem
            smooth: true
            width: button.iconSize
            height: button.iconSize
            fillMode: Image.PreserveAspectFit
            source: button.iconSource
            anchors.margins: 5
        }
        Text {
            id: textitem
            text: button.text
            anchors.margins: 4
        }

        state: {
            if (button.iconSource == "")
                return "onlytext"

            if (button.text == "")
                return "onlyicon"

            return "vertical";
        }

        states: [
            State {
                name: "vertical"

                AnchorChanges {
                    target: iconitem
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                AnchorChanges {
                    target: textitem
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                PropertyChanges {
                    target: styleitem
                    contentWidth: Math.max(textitem.paintedWidth, iconitem.paintedWidth) + 6
                    contentHeight: textitem.paintedHeight + iconitem.paintedHeight + 4
                }
            },

            State {
                name: "onlyicon"

                AnchorChanges {
                    target: iconitem
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                PropertyChanges {
                    target: textitem
                    visible: false
                }
                PropertyChanges {
                    target: styleitem
                    contentWidth: iconitem.paintedWidth + 2
                    contentHeight: iconitem.paintedHeight + 2
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
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                PropertyChanges {
                    target: styleitem
                    contentWidth: textitem.paintedWidth + 4
                    contentHeight: textitem.paintedHeight + 4
                }
            }
        ]
    }
}
