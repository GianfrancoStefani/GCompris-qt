/* Kids4Edu - LearnerLoginDialog.qml
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import core 1.0

Rectangle {
    id: learnerLoginDialog
    anchors.fill: parent
    color: "#cc000000"
    z: 2500
    visible: false

    property string selectedLearnerId: ""
    property string selectedLearnerName: ""
    property string pinInput: ""
    property string errorMessage: ""

    signal closed()

    Rectangle {
        id: card
        width: Math.min(parent.width * 0.9, 640 * ApplicationInfo.ratio)
        height: Math.min(parent.height * 0.92, 580 * ApplicationInfo.ratio)
        anchors.centerIn: parent
        radius: 16 * ApplicationInfo.ratio
        color: "#ffffff"
        border.color: "#e2e8f0"
        border.width: 2

        MouseArea {
            anchors.fill: parent
        }

        Column {
            anchors.fill: parent
            anchors.margins: 20 * ApplicationInfo.ratio
            spacing: 12 * ApplicationInfo.ratio

            // Header
            Row {
                width: parent.width
                spacing: 12 * ApplicationInfo.ratio

                Rectangle {
                    width: 44 * ApplicationInfo.ratio
                    height: 44 * ApplicationInfo.ratio
                    radius: 10 * ApplicationInfo.ratio
                    color: "#f0fdf4"

                    Text {
                        anchors.centerIn: parent
                        text: "👤"
                        font.pixelSize: 22 * ApplicationInfo.ratio
                    }
                }

                Column {
                    width: parent.width - 180 * ApplicationInfo.ratio

                    Text {
                        text: qsTr("Accesso Studente Kids4Edu")
                        font.pixelSize: 18 * ApplicationInfo.ratio
                        font.weight: Font.Bold
                        color: "#0f172a"
                    }

                    Text {
                        text: workstationManager.organizationName.length > 0 ?
                              (workstationManager.organizationName + " • " + workstationManager.workstationName) :
                              qsTr("Postazione Locale Standalone")
                        font.pixelSize: 12 * ApplicationInfo.ratio
                        color: "#64748b"
                        elide: Text.ElideRight
                        width: parent.width
                    }
                }

                // Online/Offline status badge
                Rectangle {
                    width: 110 * ApplicationInfo.ratio
                    height: 28 * ApplicationInfo.ratio
                    radius: 14 * ApplicationInfo.ratio
                    color: workstationManager.isOnline ? "#dcfce7" : "#fef3c7"
                    border.color: workstationManager.isOnline ? "#86efac" : "#fcd34d"
                    border.width: 1

                    Row {
                        anchors.centerIn: parent
                        spacing: 4 * ApplicationInfo.ratio

                        Rectangle {
                            width: 8 * ApplicationInfo.ratio
                            height: 8 * ApplicationInfo.ratio
                            radius: 4 * ApplicationInfo.ratio
                            color: workstationManager.isOnline ? "#16a34a" : "#d97706"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: workstationManager.isOnline ? qsTr("Online") : qsTr("Offline")
                            font.pixelSize: 11 * ApplicationInfo.ratio
                            font.weight: Font.Bold
                            color: workstationManager.isOnline ? "#166534" : "#92400e"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#e2e8f0"
            }

            // Main body: Split left (learner list) and right (PIN keypad)
            Row {
                width: parent.width
                height: 380 * ApplicationInfo.ratio
                spacing: 16 * ApplicationInfo.ratio

                // Left: Learner selector list
                Column {
                    width: parent.width * 0.48
                    height: parent.height
                    spacing: 8 * ApplicationInfo.ratio

                    Text {
                        text: qsTr("Seleziona il tuo profilo:")
                        font.pixelSize: 13 * ApplicationInfo.ratio
                        font.weight: Font.DemiBold
                        color: "#334155"
                    }

                    Rectangle {
                        width: parent.width
                        height: parent.height - 30 * ApplicationInfo.ratio
                        radius: 8 * ApplicationInfo.ratio
                        color: "#f8fafc"
                        border.color: "#e2e8f0"
                        border.width: 1
                        clip: true

                        ListView {
                            id: learnerList
                            anchors.fill: parent
                            anchors.margins: 4 * ApplicationInfo.ratio
                            model: workstationManager.learnerProfiles
                            spacing: 4 * ApplicationInfo.ratio

                            delegate: Rectangle {
                                width: learnerList.width
                                height: 44 * ApplicationInfo.ratio
                                radius: 6 * ApplicationInfo.ratio
                                color: selectedLearnerId === modelData.id ? "#dbeafe" : (itemMouseArea.containsMouse ? "#f1f5f9" : "#ffffff")
                                border.color: selectedLearnerId === modelData.id ? "#3b82f6" : "#e2e8f0"
                                border.width: 1

                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 8 * ApplicationInfo.ratio
                                    spacing: 8 * ApplicationInfo.ratio

                                    Rectangle {
                                        width: 28 * ApplicationInfo.ratio
                                        height: 28 * ApplicationInfo.ratio
                                        radius: 14 * ApplicationInfo.ratio
                                        color: selectedLearnerId === modelData.id ? "#3b82f6" : "#cbd5e1"

                                        Text {
                                            anchors.centerIn: parent
                                            text: modelData.display_name ? modelData.display_name.charAt(0) : "S"
                                            font.pixelSize: 13 * ApplicationInfo.ratio
                                            font.weight: Font.Bold
                                            color: "#ffffff"
                                        }
                                    }

                                    Column {
                                        anchors.verticalCenter: parent.verticalCenter
                                        Text {
                                            text: modelData.display_name || "Studente"
                                            font.pixelSize: 13 * ApplicationInfo.ratio
                                            font.weight: selectedLearnerId === modelData.id ? Font.Bold : Font.Normal
                                            color: "#1e293b"
                                        }
                                    }
                                }

                                MouseArea {
                                    id: itemMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        selectedLearnerId = modelData.id;
                                        selectedLearnerName = modelData.display_name;
                                        pinInput = "";
                                        errorMessage = "";
                                    }
                                }
                            }
                        }
                    }
                }

                // Right: PIN pad & actions
                Column {
                    width: parent.width * 0.48
                    height: parent.height
                    spacing: 10 * ApplicationInfo.ratio

                    Text {
                        text: qsTr("Inserisci PIN di accesso:")
                        font.pixelSize: 13 * ApplicationInfo.ratio
                        font.weight: Font.DemiBold
                        color: "#334155"
                    }

                    // PIN display boxes
                    Row {
                        spacing: 8 * ApplicationInfo.ratio
                        anchors.horizontalCenter: parent.horizontalCenter

                        Repeater {
                            model: 4
                            Rectangle {
                                width: 36 * ApplicationInfo.ratio
                                height: 42 * ApplicationInfo.ratio
                                radius: 8 * ApplicationInfo.ratio
                                color: "#f8fafc"
                                border.color: pinInput.length > index ? "#3b82f6" : "#cbd5e1"
                                border.width: 2

                                Text {
                                    anchors.centerIn: parent
                                    text: pinInput.length > index ? "●" : ""
                                    font.pixelSize: 20 * ApplicationInfo.ratio
                                    color: "#1e3a8a"
                                }
                            }
                        }
                    }

                    // 1-9 & 0 Keypad
                    Grid {
                        columns: 3
                        spacing: 6 * ApplicationInfo.ratio
                        anchors.horizontalCenter: parent.horizontalCenter

                        Repeater {
                            model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "C", "0", "←"]
                            Button {
                                width: 50 * ApplicationInfo.ratio
                                height: 42 * ApplicationInfo.ratio
                                text: modelData
                                font.pixelSize: 16 * ApplicationInfo.ratio
                                font.weight: Font.Bold
                                background: Rectangle {
                                    radius: 6 * ApplicationInfo.ratio
                                    color: parent.down ? "#cbd5e1" : (modelData === "C" ? "#fee2e2" : "#f1f5f9")
                                    border.color: "#e2e8f0"
                                }
                                onClicked: {
                                    errorMessage = "";
                                    if (modelData === "C") {
                                        pinInput = "";
                                    } else if (modelData === "←") {
                                        if (pinInput.length > 0) pinInput = pinInput.substring(0, pinInput.length - 1);
                                    } else {
                                        if (pinInput.length < 6) pinInput += modelData;
                                    }
                                }
                            }
                        }
                    }

                    // Error text
                    Text {
                        text: errorMessage
                        font.pixelSize: 11 * ApplicationInfo.ratio
                        color: "#dc2626"
                        wrapMode: Text.WordWrap
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        visible: text.length > 0
                    }

                    // Buttons
                    Row {
                        spacing: 8 * ApplicationInfo.ratio
                        anchors.horizontalCenter: parent.horizontalCenter

                        Button {
                            text: qsTr("Accedi")
                            height: 38 * ApplicationInfo.ratio
                            width: 100 * ApplicationInfo.ratio
                            font.pixelSize: 13 * ApplicationInfo.ratio
                            font.weight: Font.Bold
                            enabled: selectedLearnerId.length > 0 && pinInput.length > 0
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            background: Rectangle {
                                radius: 8 * ApplicationInfo.ratio
                                color: parent.enabled ? (parent.down ? "#1d4ed8" : "#2563eb") : "#94a3b8"
                            }
                            onClicked: {
                                workstationManager.verifyPinAndLogin(selectedLearnerId, pinInput);
                            }
                        }

                        Button {
                            text: qsTr("Accesso Ospite")
                            height: 38 * ApplicationInfo.ratio
                            width: 110 * ApplicationInfo.ratio
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            background: Rectangle {
                                radius: 8 * ApplicationInfo.ratio
                                color: parent.down ? "#e2e8f0" : "#f8fafc"
                                border.color: "#cbd5e1"
                            }
                            onClicked: {
                                workstationManager.loginAsGuest();
                                learnerLoginDialog.visible = false;
                                learnerLoginDialog.closed();
                            }
                        }
                    }
                }
            }
        }

        // Close button in top-right
        Button {
            text: "✕"
            width: 32 * ApplicationInfo.ratio
            height: 32 * ApplicationInfo.ratio
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 12 * ApplicationInfo.ratio
            background: Rectangle {
                radius: 16 * ApplicationInfo.ratio
                color: parent.down ? "#e2e8f0" : "#f1f5f9"
            }
            onClicked: {
                learnerLoginDialog.visible = false;
                learnerLoginDialog.closed();
            }
        }
    }

    Connections {
        target: workstationManager
        function onLoginSucceeded(id, name) {
            pinInput = "";
            errorMessage = "";
            learnerLoginDialog.visible = false;
            learnerLoginDialog.closed();
        }
        function onLoginFailed(msg) {
            errorMessage = msg;
            pinInput = "";
        }
    }
}
