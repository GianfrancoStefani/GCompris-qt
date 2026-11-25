/* Kids4Edu - DiagnosticsDialog.qml
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import core 1.0

Rectangle {
    id: diagnosticsDialog
    anchors.fill: parent
    color: "#cc000000"
    z: 2500
    visible: false

    signal closed()
    signal requestEnrollment()
    signal requestSwitchLearner()

    Rectangle {
        id: card
        width: Math.min(parent.width * 0.88, 660 * ApplicationInfo.ratio)
        height: Math.min(parent.height * 0.92, 560 * ApplicationInfo.ratio)
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
            anchors.margins: 22 * ApplicationInfo.ratio
            spacing: 12 * ApplicationInfo.ratio

            // Header
            Row {
                width: parent.width
                spacing: 12 * ApplicationInfo.ratio

                Rectangle {
                    width: 44 * ApplicationInfo.ratio
                    height: 44 * ApplicationInfo.ratio
                    radius: 10 * ApplicationInfo.ratio
                    color: "#f1f5f9"

                    Text {
                        anchors.centerIn: parent
                        text: "🛠️"
                        font.pixelSize: 22 * ApplicationInfo.ratio
                    }
                }

                Column {
                    width: parent.width - 150 * ApplicationInfo.ratio

                    Text {
                        text: qsTr("Diagnostica Postazione & Sincronizzazione")
                        font.pixelSize: 18 * ApplicationInfo.ratio
                        font.weight: Font.Bold
                        color: "#0f172a"
                    }

                    Text {
                        text: qsTr("Pannello operatore scolastico Kids4Edu Desktop v2.1.0")
                        font.pixelSize: 12 * ApplicationInfo.ratio
                        color: "#64748b"
                    }
                }

                // Close button
                Button {
                    text: "✕"
                    width: 32 * ApplicationInfo.ratio
                    height: 32 * ApplicationInfo.ratio
                    background: Rectangle {
                        radius: 16 * ApplicationInfo.ratio
                        color: parent.down ? "#e2e8f0" : "#f8fafc"
                    }
                    onClicked: {
                        diagnosticsDialog.visible = false;
                        diagnosticsDialog.closed();
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#e2e8f0"
            }

            // Info Grid
            Grid {
                columns: 2
                spacing: 12 * ApplicationInfo.ratio
                width: parent.width

                // Box 1: Workstation & Organization
                Rectangle {
                    width: (parent.width - 12 * ApplicationInfo.ratio) / 2
                    height: 130 * ApplicationInfo.ratio
                    radius: 10 * ApplicationInfo.ratio
                    color: "#f8fafc"
                    border.color: "#e2e8f0"
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10 * ApplicationInfo.ratio
                        spacing: 4 * ApplicationInfo.ratio

                        Text {
                            text: qsTr("Identità Postazione")
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            font.weight: Font.Bold
                            color: "#475569"
                        }

                        Text {
                            text: qsTr("Nome: ") + (workstationManager.workstationName || "Non configurata")
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            font.weight: Font.Medium
                            color: "#0f172a"
                        }

                        Text {
                            text: qsTr("Org: ") + (workstationManager.organizationName || "Standalone")
                            font.pixelSize: 11 * ApplicationInfo.ratio
                            color: "#64748b"
                            elide: Text.ElideRight
                            width: parent.width
                        }

                        Text {
                            text: qsTr("Stato: ") + (workstationManager.isEnrolled ? "Registrata" : "Non Registrata")
                            font.pixelSize: 11 * ApplicationInfo.ratio
                            color: workstationManager.isEnrolled ? "#16a34a" : "#dc2626"
                            font.weight: Font.Bold
                        }

                        Text {
                            text: qsTr("Backend: ") + workstationManager.backendUrl
                            font.pixelSize: 10 * ApplicationInfo.ratio
                            color: "#94a3b8"
                            elide: Text.ElideMiddle
                            width: parent.width
                        }
                    }
                }

                // Box 2: Current Learner & Session
                Rectangle {
                    width: (parent.width - 12 * ApplicationInfo.ratio) / 2
                    height: 130 * ApplicationInfo.ratio
                    radius: 10 * ApplicationInfo.ratio
                    color: "#f8fafc"
                    border.color: "#e2e8f0"
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10 * ApplicationInfo.ratio
                        spacing: 4 * ApplicationInfo.ratio

                        Text {
                            text: qsTr("Profilo Studente Attivo")
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            font.weight: Font.Bold
                            color: "#475569"
                        }

                        Text {
                            text: workstationManager.currentLearnerName
                            font.pixelSize: 14 * ApplicationInfo.ratio
                            font.weight: Font.Bold
                            color: "#1e3a8a"
                        }

                        Text {
                            text: qsTr("ID: ") + workstationManager.currentLearnerId
                            font.pixelSize: 10 * ApplicationInfo.ratio
                            color: "#64748b"
                            elide: Text.ElideMiddle
                            width: parent.width
                        }

                        Text {
                            text: qsTr("Connettività: ") + (workstationManager.isOnline ? "Online (Connesso)" : "Offline (Resiliente)")
                            font.pixelSize: 11 * ApplicationInfo.ratio
                            color: workstationManager.isOnline ? "#16a34a" : "#d97706"
                            font.weight: Font.DemiBold
                        }

                        Text {
                            text: qsTr("Scadenza Offline: 72 ore")
                            font.pixelSize: 10 * ApplicationInfo.ratio
                            color: "#94a3b8"
                        }
                    }
                }
            }

            // Sync Stats Box
            Rectangle {
                width: parent.width
                height: 120 * ApplicationInfo.ratio
                radius: 10 * ApplicationInfo.ratio
                color: "#f0fdf4"
                border.color: "#bbf7d0"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 12 * ApplicationInfo.ratio
                    spacing: 6 * ApplicationInfo.ratio

                    Row {
                        width: parent.width
                        Text {
                            text: qsTr("Pipeline Telemetria SQLite WAL → SyncAgent → PostgreSQL")
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            font.weight: Font.Bold
                            color: "#166534"
                        }
                    }

                    Row {
                        width: parent.width
                        spacing: 20 * ApplicationInfo.ratio

                        Column {
                            Text {
                                text: "" + syncAgent.pendingCount
                                font.pixelSize: 22 * ApplicationInfo.ratio
                                font.weight: Font.Bold
                                color: syncAgent.pendingCount > 0 ? "#d97706" : "#166534"
                            }
                            Text {
                                text: qsTr("In attesa (SQLite)")
                                font.pixelSize: 11 * ApplicationInfo.ratio
                                color: "#4b5563"
                            }
                        }

                        Column {
                            Text {
                                text: "" + syncAgent.inFlightCount
                                font.pixelSize: 22 * ApplicationInfo.ratio
                                font.weight: Font.Bold
                                color: "#2563eb"
                            }
                            Text {
                                text: qsTr("In trasmissione")
                                font.pixelSize: 11 * ApplicationInfo.ratio
                                color: "#4b5563"
                            }
                        }

                        Column {
                            Text {
                                text: "" + syncAgent.syncedCount
                                font.pixelSize: 22 * ApplicationInfo.ratio
                                font.weight: Font.Bold
                                color: "#16a34a"
                            }
                            Text {
                                text: qsTr("Sincronizzati con ACK")
                                font.pixelSize: 11 * ApplicationInfo.ratio
                                color: "#4b5563"
                            }
                        }

                        Column {
                            Text {
                                text: syncAgent.lastSyncTime.length > 0 ? syncAgent.lastSyncTime : qsTr("Mai")
                                font.pixelSize: 12 * ApplicationInfo.ratio
                                font.weight: Font.Medium
                                color: "#1e293b"
                            }
                            Text {
                                text: qsTr("Ultima sincronizzazione")
                                font.pixelSize: 11 * ApplicationInfo.ratio
                                color: "#64748b"
                            }
                        }
                    }
                }
            }

            // Action Buttons Row
            Row {
                width: parent.width
                spacing: 10 * ApplicationInfo.ratio
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: qsTr("🔄 Sincronizza Ora")
                    font.pixelSize: 12 * ApplicationInfo.ratio
                    font.weight: Font.Bold
                    height: 38 * ApplicationInfo.ratio
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        color: parent.down ? "#15803d" : "#16a34a"
                    }
                    onClicked: {
                        syncAgent.triggerSyncNow();
                        workstationManager.fetchProfiles();
                    }
                }

                Button {
                    text: qsTr("👤 Cambia Studente")
                    font.pixelSize: 12 * ApplicationInfo.ratio
                    height: 38 * ApplicationInfo.ratio
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        color: parent.down ? "#dbeafe" : "#eff6ff"
                        border.color: "#bfdbfe"
                    }
                    onClicked: {
                        diagnosticsDialog.visible = false;
                        diagnosticsDialog.requestSwitchLearner();
                    }
                }

                Button {
                    text: workstationManager.isEnrolled ? qsTr("⚙️ Riconfigura Postazione") : qsTr("🏫 Attiva Postazione")
                    font.pixelSize: 12 * ApplicationInfo.ratio
                    height: 38 * ApplicationInfo.ratio
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        color: parent.down ? "#e2e8f0" : "#f1f5f9"
                        border.color: "#cbd5e1"
                    }
                    onClicked: {
                        diagnosticsDialog.visible = false;
                        diagnosticsDialog.requestEnrollment();
                    }
                }

                Button {
                    text: qsTr("Deregistra")
                    font.pixelSize: 12 * ApplicationInfo.ratio
                    height: 38 * ApplicationInfo.ratio
                    visible: workstationManager.isEnrolled
                    background: Rectangle {
                        radius: 8 * ApplicationInfo.ratio
                        color: parent.down ? "#fecaca" : "#fee2e2"
                        border.color: "#fca5a5"
                    }
                    onClicked: {
                        workstationManager.unenroll();
                    }
                }
            }
        }
    }
}
