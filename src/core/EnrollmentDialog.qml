/* Kids4Edu - EnrollmentDialog.qml
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import core 1.0

Rectangle {
    id: enrollmentDialog
    anchors.fill: parent
    color: "#cc000000"
    z: 2500
    visible: false

    signal closed()

    Rectangle {
        id: card
        width: Math.min(parent.width * 0.85, 620 * ApplicationInfo.ratio)
        height: Math.min(parent.height * 0.9, 520 * ApplicationInfo.ratio)
        anchors.centerIn: parent
        radius: 16 * ApplicationInfo.ratio
        color: "#ffffff"
        border.color: "#e2e8f0"
        border.width: 2

        MouseArea {
            anchors.fill: parent
            // Prevent clicks from dismissing through
        }

        Column {
            anchors.fill: parent
            anchors.margins: 24 * ApplicationInfo.ratio
            spacing: 14 * ApplicationInfo.ratio

            Row {
                width: parent.width
                spacing: 12 * ApplicationInfo.ratio

                Rectangle {
                    width: 44 * ApplicationInfo.ratio
                    height: 44 * ApplicationInfo.ratio
                    radius: 10 * ApplicationInfo.ratio
                    color: "#e0f2fe"

                    Text {
                        anchors.centerIn: parent
                        text: "🏫"
                        font.pixelSize: 24 * ApplicationInfo.ratio
                    }
                }

                Column {
                    width: parent.width - 60 * ApplicationInfo.ratio

                    Text {
                        text: qsTr("Attivazione Postazione Scolastica")
                        font.pixelSize: 20 * ApplicationInfo.ratio
                        font.weight: Font.Bold
                        color: "#0f172a"
                    }

                    Text {
                        text: qsTr("Connetti questa postazione a Kids4Edu Management Portal")
                        font.pixelSize: 13 * ApplicationInfo.ratio
                        color: "#64748b"
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#e2e8f0"
            }

            // Backend URL
            Column {
                width: parent.width
                spacing: 4 * ApplicationInfo.ratio

                Text {
                    text: qsTr("URL Backend Kids4Edu:")
                    font.pixelSize: 13 * ApplicationInfo.ratio
                    font.weight: Font.Medium
                    color: "#334155"
                }

                TextField {
                    id: backendUrlField
                    width: parent.width
                    height: 38 * ApplicationInfo.ratio
                    text: workstationManager.backendUrl
                    placeholderText: "http://localhost:8000"
                    font.pixelSize: 14 * ApplicationInfo.ratio
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        border.color: backendUrlField.activeFocus ? "#3b82f6" : "#cbd5e1"
                        border.width: 1
                        color: "#f8fafc"
                    }
                }
            }

            // Workstation Name
            Column {
                width: parent.width
                spacing: 4 * ApplicationInfo.ratio

                Text {
                    text: qsTr("Nome Postazione (es. Aula-1-PC05):")
                    font.pixelSize: 13 * ApplicationInfo.ratio
                    font.weight: Font.Medium
                    color: "#334155"
                }

                TextField {
                    id: wsNameField
                    width: parent.width
                    height: 38 * ApplicationInfo.ratio
                    text: workstationManager.workstationName.length > 0 ? workstationManager.workstationName : "Lab-PC-01"
                    placeholderText: "Nome identificativo della postazione"
                    font.pixelSize: 14 * ApplicationInfo.ratio
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        border.color: wsNameField.activeFocus ? "#3b82f6" : "#cbd5e1"
                        border.width: 1
                        color: "#f8fafc"
                    }
                }
            }

            // Enrollment Token
            Column {
                width: parent.width
                spacing: 4 * ApplicationInfo.ratio

                Text {
                    text: qsTr("Token di Attivazione (generato da Admin Portal):")
                    font.pixelSize: 13 * ApplicationInfo.ratio
                    font.weight: Font.Medium
                    color: "#334155"
                }

                TextField {
                    id: tokenField
                    width: parent.width
                    height: 38 * ApplicationInfo.ratio
                    placeholderText: "k4e_act_..."
                    font.pixelSize: 14 * ApplicationInfo.ratio
                    echoMode: TextInput.Normal
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        border.color: tokenField.activeFocus ? "#3b82f6" : "#cbd5e1"
                        border.width: 1
                        color: "#f8fafc"
                    }
                }
            }

            // Status message
            Text {
                id: statusMsg
                width: parent.width
                wrapMode: Text.WordWrap
                text: workstationManager.statusMessage
                font.pixelSize: 12 * ApplicationInfo.ratio
                color: text.indexOf("Errore") !== -1 ? "#dc2626" : (text.indexOf("successo") !== -1 ? "#16a34a" : "#2563eb")
                visible: text.length > 0
            }

            Item {
                width: parent.width
                height: 10 * ApplicationInfo.ratio
            }

            // Action buttons
            Row {
                width: parent.width
                spacing: 12 * ApplicationInfo.ratio
                layoutDirection: Qt.RightToLeft

                Button {
                    text: qsTr("Attiva Postazione")
                    font.pixelSize: 14 * ApplicationInfo.ratio
                    font.weight: Font.Bold
                    height: 40 * ApplicationInfo.ratio
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        color: parent.down ? "#1d4ed8" : "#2563eb"
                    }
                    onClicked: {
                        workstationManager.enroll(backendUrlField.text, tokenField.text, wsNameField.text);
                    }
                }

                Button {
                    text: qsTr("Annulla / Modalità Standalone")
                    font.pixelSize: 14 * ApplicationInfo.ratio
                    height: 40 * ApplicationInfo.ratio
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "#475569"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        border.color: "#cbd5e1"
                        border.width: 1
                        color: parent.down ? "#e2e8f0" : "#f1f5f9"
                    }
                    onClicked: {
                        enrollmentDialog.visible = false;
                        enrollmentDialog.closed();
                    }
                }
            }
        }
    }

    Connections {
        target: workstationManager
        function onEnrollmentSucceeded(name) {
            tokenField.text = "";
            enrollmentDialog.visible = false;
            enrollmentDialog.closed();
        }
    }
}
