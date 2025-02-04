import QtQuick
import QtQuick.Controls.Material

ApplicationWindow {
    id: root
    minimumWidth: 250
    minimumHeight: 350
    width: 380
    height: 580
    visible: true
    title: qsTr("Set Own Icon")

    // onHeightChanged: {
    //     console.log("w: "+width + ", h: ", height);
    // }
    // onWidthChanged: {
    //     console.log("w: "+width + ", h: ", height);
    // }

    readonly property color fileEnterColor: "lightblue"
    readonly property color fileExitColor: "lightgray"
    property color fileColor: fileExitColor

    readonly property color fileSelectedColor: "lightgreen"

    readonly property color iconEnterColor: "lightblue"
    readonly property color iconExitColor: "lightgray"
    property color iconColor: iconExitColor

    readonly property color iconSelectedColor: "lightgreen"


    function handleDrop(droppedFile, type){
        if(type === "file")
        {
            console.log("set selected file: " + droppedFile)
            Backend.setSelectedFile(droppedFile)
        }
        else // if type === "icon"
        {
            console.log("set selected icon: " + droppedFile)
            Backend.setSelectedIcon(droppedFile)
        }
    }

    function handleDroppes(droppedFiles, type){
        if(droppedFiles.length === 0)
        {
            console.error("no files were dropped!");
            // no files => error
        }
        else if(droppedFiles.length === 1)
        {
            handleDrop(droppedFiles[0], type);
        }
        else
        {
            console.error("to much files were dropped ("+ droppedFiles.length +")!");
            // for (var i = 0; i < droppedFiles.length; i++) {
            //     console.log(drop.urls[i]);
            // }

            // to much files => warning
        }
    }

    Item{
        id: fileArea
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 10
        }
        height: parent.height * 0.35
        clip: true

        Rectangle{
            id: fileAreaReact
            anchors{
                fill: parent
            }

            color: {
                if(Backend.isFileSelected)
                    root.fileSelectedColor
                else
                    root.fileColor
            }
        }

        DropArea{
            id: fileDropArea
            anchors.fill: parent

            onEntered: {
                console.log("files entered top area")

                root.fileColor = root.fileEnterColor;

            }

            onExited: {
                console.log("files exited top area")

                root.fileColor = root.fileExitColor;

            }
            onDropped: (drop) => {
                handleFileDrop(drop);
            }

            function handleFileDrop(drop) {
                console.log("files are dropped");

                handleDroppes(drop.urls, "file");

                root.fileColor = root.fileExitColor;
            }
        }

        Label{
            id: folderAreaLabel
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: "Drop Folder Here"
            font.pixelSize: 20
            // font.bold: true
        }
        Label{
            anchors{
                top: folderAreaLabel.bottom
                left: parent.left
                right: parent.right
            }
            visible: Backend.isFileSelected

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WrapAnywhere

            text: Backend.selectedFile
            font.pixelSize: 14
            // font.bold: true
        }
    }

    Item{
        id: iconArea
        anchors{
            top: fileArea.bottom
            bottom: confirmButtonArea.top
            left: parent.left
            right: parent.right
            margins: 10
        }
        clip: true

        Rectangle{
            id: itemAreaReact
            anchors{
                fill: parent
            }

            color: {
                if(Backend.isIconSelected)
                    root.iconSelectedColor
                else
                    root.iconColor
            }
        }

        DropArea{
            id: iconDropArea
            anchors.fill: parent

            onEntered: {
                console.log("files entered top area")

                root.iconColor = root.iconEnterColor;

            }

            onExited: {
                console.log("files exited top area")

                root.iconColor = root.iconExitColor;

            }
            onDropped: (drop) => {
                handleItemDrop(drop);
            }

            function handleItemDrop(drop) {
                console.log("files are dropped");

                handleDroppes(drop.urls, "icon");

                root.iconColor = root.iconExitColor;
            }
        }

        Label{
            id: iconAreaLabel
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: "Drop Icon Here"
            font.pixelSize: 20
            // font.bold: true
        }
        Label{
            anchors{
                top: iconAreaLabel.bottom
                left: parent.left
                right: parent.right
            }
            visible: Backend.isIconSelected

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WrapAnywhere

            text: Backend.selectedIcon
            font.pixelSize: 14
            // font.bold: true
        }

    }

    Item{
        id: confirmButtonArea
        anchors{
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: 60

        Button{
            anchors.centerIn: parent
            text: "Set Icon"
            enabled: Backend.isFileSelected && Backend.isIconSelected
            onClicked: {
                Backend.setIconForFile();
            }
        }
    }

}
