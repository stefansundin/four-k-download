import QtQuick 1.1
import "qrc:/gui/qml/desktop" as Desktop
import Desktop 0.1 as Desktop
import ComponentModel 1.0 as ComponentModel
import Gui 1.0 as Gui
import Mvvm 1.0 as Mvvm
import Application 1.0 as Application

Desktop.ApplicationWindow {
    id: view

    property variant viewModel: dataContext

    // Dialog

    property string dialogTitle: "4K Video to MP3"
    property string dialogIcon: ":/image/application-icon"
    property bool dialogMinimizeButton: true
    property bool dialogMaximizeButton: true
    property int dialogMinWidth: 500
    property int dialogMinHeight: 400
    property list<Item> dialogToolBar

    width: 550
    height: 750

    function dialogCanClose() {
        if (view.viewModel.isIdle())
            return true;

        var index = dialog.showMessageDialog(qsTr("Some conversions are not finished yet. Are you sure want to quit?"), Mvvm.Dialog.QuestionIcon,
                                             "", [qsTr("Quit"), qsTr("Cancel")], 0);
        return index === 0;
    }

    // Auxilary objects

    Mvvm.Dialog {id: dialog}
    ComponentModel.Transform {id: dataTransform}
    SystemPalette {id: systemPallete}
    Desktop.StyleItem {id: styleItem}

    Text {
        id: textTemplate
        property string alternativeFontFamily: isWindowsVista() ? "Segoe UI" : font.family
        property int alternativeFontPixelSize: isWindows() ? font.pixelSize + 1 : font.pixelSize
    }

    // Auxilary functions

    function addItems() {
        var paths = dialog.showOpenDialog(false);
        for (var i = 0; i < paths.length; ++i)
            view.viewModel.addItem(paths[i]);
    }

    function changeOutputPath() {
        var path = dialog.showDirectoryDialog(view.viewModel.outputPath);
        if (path.length > 0)
            view.viewModel.outputPath = path;
    }

    function showMessage(message) {
        dialog.showMessageDialog(message, Mvvm.Dialog.InformationIcon,
                                 "", [qsTr("Close")]);
    }

    function showUpdate(message, retVal) {
        var index = dialog.showMessageDialog(message, Mvvm.Dialog.InformationIcon,
                                             "", [qsTr("Open Site"), qsTr("Close")], 0);
        retVal.needUpdate = (index === 0);
    }

    function stringFormat(str, args) {
        for(var i = 0; i < args.length; ++i)
            str = str.replace(eval("/%"+(i+1)+"/"), args[i]);
        return str;
    }

    function isWindows() {
        var patt = /.*windows.*/;
        return patt.test(styleItem.style);
    }

    function isWindowsVista() {
        var patt = /.*vista.*/;
        return patt.test(styleItem.style);
    }

    function isMac() {
        return styleItem.style === "mac";
    }

    Component.onCompleted: {
        view.viewModel.messageReceived.connect(showMessage)
        view.viewModel.updateReceived.connect(showUpdate)
    }

    // Layout

    dialogToolBar: [
        Item {
            property string text: qsTr("Add Video")
            property string tooltip: qsTr("Add video to extract audio to mp3")
            property string iconSource: ":/image/add"
            property int iconSize: 32
            function trigger() {
                addItems()
            }
        },
        Item {
            property string text: qsTr("Change Folder")
            property string tooltip: qsTr("Change folder for output audio files")
            property string iconSource: ":/image/change-folder"
            function trigger() {
                changeOutputPath()
            }
        },
        Item {
            property string text: "|"
        },
        Item {
            property string text: qsTr("Help")
            property string tooltip: qsTr("Show online help")
            property string iconSource: ":/image/help"
            function trigger() {
                view.viewModel.showHelp()
            }
        },
        Item {
            property string text: "-"
        },
        Item {
            property string text: qsTr("Like")
            property string tooltip: qsTr("Show 4K Download Facebook page")
            property string iconSource: ":/image/facebook"
            function trigger() {
                view.viewModel.showFacebook()
            }
        }
    ]

    Component {
        id: mediaItemDelegate

        Rectangle {
            id: root
            width: ListView.view.width
            height: thumbnail.height + 12
            color: {
                if (ListView.isCurrentItem)
                    return systemPallete.highlight;
                else if (index % 2 === 1)
                    return systemPallete.alternateBase;
                else
                    return "transparent";
            }

            // Key and mouse

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                hoverEnabled: true
                onPressed: parent.ListView.view.currentIndex = index;
                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        var c = view.mapFromItem(mouseArea, mouse.x, mouse.y)
                        contextMenu.clickedObject = model.object
                        contextMenu.showPopup(c.x, c.y)
                    }
                }
                onDoubleClicked: {
                    if (model.object.togglePlayAction.enabled)
                        model.object.togglePlayAction.trigger();
                }
            }

            // Thumbnail

            Image {
                id: thumbnail
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 6
                }
                width: 80
                height: 45
                fillMode: Image.PreserveAspectCrop
                clip: true
                smooth: true
                source: model.object.thumbnail.length > 0 ? "image://thumbnails/" + model.object.thumbnail : ""
                visible: model.object.thumbnail.length > 0
            }

            Image {
                id: togglePlayerButton
                anchors.centerIn: thumbnail
                width: 32
                height: 32
                smooth: true
                source: model.object.state === Application.MediaItemViewModel.PlayerPlayState ? ":/image/item-logo-pause" : ":/image/item-logo-play"
                visible: thumbnail.visible && opacity > 0.0 && model.object.togglePlayAction.enabled
                opacity: {
                    if (togglePlayerButtonMouseArea.containsMouse)
                        return 0.7
                    else if (model.object.state === Application.MediaItemViewModel.DoneState ||
                             model.object.state === Application.MediaItemViewModel.PlayerPlayState ||
                             model.object.state === Application.MediaItemViewModel.PlayerPauseState)
                        return 0.4

                    return 0.0;
                }

                Behavior on opacity {
                    SmoothedAnimation { velocity: -1; duration: 100 }
                }

                MouseArea {
                    id: togglePlayerButtonMouseArea
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    hoverEnabled: true
                    onClicked: {
                        if (model.object.togglePlayAction.enabled)
                            model.object.togglePlayAction.trigger();
                    }
                }

                Gui.CursorShapeArea {
                    anchors.fill: parent
                    visible: isMac()
                    cursorShape: Qt.PointingHandCursor
                }
            }

            Rectangle {
                width: 80
                height: 45
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 6
                }
                color: "#FFFFFF"
                radius: 2
                visible: model.object.thumbnail.length === 0

                AnimatedImage {
                    x: 23
                    y: 6
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: ":/image/progress"
                    paused: model.object.thumbnail.length > 0
                }
            }

            // Information

            Item {
                anchors {
                    left: thumbnail.right
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                    leftMargin: isMac() ? 12 : 9
                    rightMargin: deleteButton.opacity == 1.0 ? deleteButton.width + 14 : 8
                }

                Text {
                    id: titleText
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 9
                    anchors.right: parent.right
                    font.family: textTemplate.alternativeFontFamily
                    font.pixelSize: textTemplate.alternativeFontPixelSize
                    font.bold: true//isMac() ? false : true
                    color: root.ListView.isCurrentItem ? systemPallete.highlightedText : systemPallete.text
                    elide: Text.ElideRight
                    text: model.object.title
                }

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: -3
                    anchors.top: titleText.bottom
                    anchors.topMargin: 7
                    spacing: 12

                    // Duration

                    Item {
                        height: durationImage.height
                        width: durationText.x + durationText.width
                        visible: {
                            return model.object.state === Application.MediaItemViewModel.ConvertState ||
                                   model.object.state === Application.MediaItemViewModel.PauseState ||
                                   model.object.state === Application.MediaItemViewModel.DoneState ||
                                   model.object.state === Application.MediaItemViewModel.PlayerPauseState
                        }

                        Image {
                            id: durationImage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.topMargin: -1
                            source: ":/image/item-time"
                        }

                        Text {
                            id: durationText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: durationImage.right
                            anchors.leftMargin: 6
                            font.family: textTemplate.alternativeFontFamily
                            font.pixelSize: textTemplate.alternativeFontPixelSize
                            color: root.ListView.isCurrentItem ? systemPallete.highlightedText : Qt.darker(systemPallete.window)
                            text: dataTransform.timeToString(model.object.duration, "h:mm:ss");
                        }
                    }

                    // Status

                    Item {
                        height: statusImage.height
                        width: {
                            if (statusProgressText.visible)
                                return statusProgressText.x + statusProgressText.width;

                            if (statusProgress.visible)
                                return statusProgress.x + statusProgress.width;

                            return statusText.x + statusText.width
                        }
                        visible: model.object.state !== Application.MediaItemViewModel.NoState

                        Image {
                            id: statusImage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.topMargin: -1
                            source: {
                                switch (model.object.state) {
                                case Application.MediaItemViewModel.OpenState:
                                case Application.MediaItemViewModel.ConvertState:
                                case Application.MediaItemViewModel.PauseState:
                                    return ":/image/item-convert";

                                case Application.MediaItemViewModel.DoneState:
                                case Application.MediaItemViewModel.PlayerPauseState:
                                    return ":/image/item-done";

                                case Application.MediaItemViewModel.PlayerPlayState:
                                    return ":/image/item-playing";

                                case Application.MediaItemViewModel.ErrorState:
                                    return ":/image/item-error";

                                default:
                                    return "";
                                }
                            }
                        }

                        Text {
                            id: statusText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: statusImage.right
                            anchors.leftMargin: 6
                            font.family: textTemplate.alternativeFontFamily
                            font.pixelSize: textTemplate.alternativeFontPixelSize
                            color: root.ListView.isCurrentItem ? systemPallete.highlightedText : Qt.darker(systemPallete.window)
                            text: {
                                switch (model.object.state) {
                                case Application.MediaItemViewModel.OpenState:
                                    return qsTr("Opening...");

                                case Application.MediaItemViewModel.DoneState:
                                case Application.MediaItemViewModel.PlayerPauseState:
                                    return qsTr("Done");

                                case Application.MediaItemViewModel.PlayerPlayState:
                                    return dataTransform.timeToString(model.object.playingTime, "h:mm:ss") + " / " + dataTransform.timeToString(model.object.duration, "h:mm:ss");

                                case Application.MediaItemViewModel.ErrorState:
                                    return qsTr("Error");

                                default:
                                    return "";
                                }
                            }
                            visible: {
                                return model.object.state === Application.MediaItemViewModel.OpenState ||
                                       model.object.state === Application.MediaItemViewModel.DoneState ||
                                       model.object.state === Application.MediaItemViewModel.PlayerPlayState ||
                                       model.object.state === Application.MediaItemViewModel.PlayerPauseState ||
                                       model.object.state === Application.MediaItemViewModel.ErrorState;
                            }
                        }

                        Desktop.ProgressBar {
                            id: statusProgress
                            anchors.top: statusImage.top
                            anchors.left: statusImage.right
                            anchors.leftMargin: 6
                            width: 200
                            height: statusImage.height
                            minimumValue: 0
                            maximumValue: 100
                            value: model.object.progress
                            visible: {
                                return model.object.state === Application.MediaItemViewModel.ConvertState ||
                                       model.object.state === Application.MediaItemViewModel.PauseState;
                            }

                            MouseArea {
                                id: progressMouseArea
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton
                                hoverEnabled: true
                                onPressed: root.ListView.view.currentIndex = index;
                            }
                        }

                        Text {
                            id: statusProgressText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: statusProgress.right
                            anchors.leftMargin: 6
                            font.family: textTemplate.alternativeFontFamily
                            font.pixelSize: textTemplate.alternativeFontPixelSize
                            color: root.ListView.isCurrentItem ? systemPallete.highlightedText : systemPallete.text
                            text: dataTransform.timeToString(model.object.remainingTime, "remain");
                            visible: statusProgress.visible && model.object.remainingTime !== -1
                        }
                    }

                }
            }

            // Buttons

            Desktop.SemitransparentButton {
                id: deleteButton
                anchors {
                    top: parent.top
                    right: parent.right
                    topMargin: 4
                    rightMargin: 7
                }
                iconSource: ":/image/item-remove"
                opacity: mouseArea.containsMouse || progressMouseArea.containsMouse || togglePlayerButtonMouseArea.containsMouse || deleteButton.containsMouse || actionButton.containsMouse ? 1.0 : 0.0
                onClicked: {
                    parent.ListView.view.currentIndex = index;
                    view.viewModel.deleteItemAction.trigger();
                }

                Gui.CursorShapeArea {
                    anchors.fill: parent
                    visible: isMac()
                    cursorShape: Qt.PointingHandCursor
                }
            }

            Desktop.SemitransparentButton {
                id: actionButton
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    bottomMargin: 6
                    rightMargin: 7
                }
                highlightTextColor: parent.ListView.isCurrentItem
                textBold: true
                text: {
                    if (model.object.pauseAction.enabled)
                        return qsTr("Pause");

                    if (model.object.resumeAction.enabled)
                        return qsTr("Resume");

                    if (model.object.showOutputAction.enabled)
                        return qsTr("Show in folder");

                    return "";
                }
                visible: actionButton.text.length > 0
                opacity: mouseArea.containsMouse || progressMouseArea.containsMouse || togglePlayerButtonMouseArea.containsMouse || deleteButton.containsMouse || actionButton.containsMouse ? 1.0 : 0.0
                onClicked: {
                    if (model.object.pauseAction.enabled) {
                        model.object.pauseAction.trigger();
                        return;
                    }

                    if (model.object.resumeAction.enabled) {
                        model.object.resumeAction.trigger();
                        return;
                    }

                    if (model.object.showOutputAction.enabled) {
                        model.object.showOutputAction.trigger();
                        return;
                    }
                }

                Gui.CursorShapeArea {
                    anchors.fill: parent
                    visible: isMac()
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            contextMenu.clickedObject = null
            contextMenu.showPopup(mouse.x, mouse.y)
        }
    }

    Desktop.ScrollArea {
        id: mediaListArea
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        frame: false
        highlightOnFocus: true
        horizontalScrollBar.visible: false
        color: systemPallete.base

        ListView {
            id: mediaListView
            width: mediaListArea.viewportWidth
            height: count * (45 + 12)
            focus: true
            Keys.onUpPressed: mediaListView.decrementCurrentIndex()
            Keys.onDownPressed: mediaListView.incrementCurrentIndex()
            interactive: false
            model: view.viewModel.mediaListModel
            delegate: mediaItemDelegate
            currentIndex: view.viewModel.mediaIndex
            Binding {
                target: view.viewModel
                property: "mediaIndex"
                value: mediaListView.currentIndex
            }
        }
    }


    Item {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
        }
        visible: mediaListView.count === 0
        width: 300
        height: parent.height * 9 / 10

        Image {
            id: messageImage
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            source: ":/image/add"
        }

        Text {
            id: messageText
            anchors {
                left: messageImage.right
                verticalCenter: parent.verticalCenter
                right: parent.right
                leftMargin: 14
            }
            font.family: textTemplate.alternativeFontFamily
            font.pixelSize: textTemplate.alternativeFontPixelSize
            wrapMode: Text.Wrap
            text: qsTr('Click "<a href=/"add-video/">Add Video</a>" button or drop video files here and conversion will start immediately.')

            MouseArea {
                anchors.fill: parent
                enabled: parent.visible
                onClicked: addItems()
            }

            Gui.CursorShapeArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    Gui.DropArea {
        anchors.fill: mediaListArea
        onFileDropped: view.viewModel.addItem(path)
    }

    Desktop.ContextMenu {
        id: contextMenu
        property variant clickedObject

        Desktop.MenuItem {
            id: menuAction
            text: {
                if (contextMenu.clickedObject) {
                    if (contextMenu.clickedObject.pauseAction.enabled)
                        return qsTr("Pause");

                    if (contextMenu.clickedObject.resumeAction.enabled)
                        return qsTr("Resume");
                }

                return "";
            }
            visible: menuAction.text.length > 0
            onTriggered: {
                if (contextMenu.clickedObject) {
                    if (contextMenu.clickedObject.pauseAction.enabled) {
                        contextMenu.clickedObject.pauseAction.trigger();
                        return;
                    }

                    if (contextMenu.clickedObject.resumeAction.enabled) {
                        contextMenu.clickedObject.resumeAction.trigger();
                        return;
                    }
                }
            }
        }

        Desktop.MenuItem {
            id: menuShowInFolder
            text: qsTr("Show in folder")
            visible: contextMenu.clickedObject ? contextMenu.clickedObject.showOutputAction.enabled : false;
            onTriggered: {
                if (contextMenu.clickedObject)
                    contextMenu.clickedObject.showOutputAction.trigger();
            }
        }

        Desktop.Separator {}

        Desktop.MenuItem {
            id: menuPauseAll
            text: qsTr("Pause All")
            visible: view.viewModel.pauseItemsAction.enabled
            onTriggered: view.viewModel.pauseItemsAction.trigger()
        }

        Desktop.MenuItem {
            id: menuResumeAll
            text: qsTr("Resume All")
            visible: view.viewModel.resumeItemsAction.enabled
            onTriggered: view.viewModel.resumeItemsAction.trigger()
        }

        Desktop.Separator {}

        Desktop.MenuItem {
            id: menuAdd
            iconSource: ":/image/add"
            text: qsTr("Add Files")
            onTriggered: addItems()
        }

        Desktop.MenuItem {
            id: menuRemove
            iconSource: ":/image/remove"
            text: qsTr("Remove")
            enabled: view.viewModel.deleteItemAction.enabled
            onTriggered: view.viewModel.deleteItemAction.trigger()
        }

        Desktop.MenuItem {
            id: menuRemoveAll
            text: qsTr("Remove All")
            enabled: view.viewModel.clearItemsAction.enabled
            onTriggered: view.viewModel.clearItemsAction.trigger()
        }
    }
}
