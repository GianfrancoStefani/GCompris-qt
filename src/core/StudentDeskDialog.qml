/* Kids4Edu - StudentDeskDialog.qml
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core 1.0

Rectangle {
    id: studentDeskDialog
    anchors.fill: parent
    color: "#f8fafc"
    z: 2200
    visible: false

    property var deskData: null
    property string activePathId: ""
    property var activeStep: null
    property bool isClassroomMode: true
    property string statusNotice: ""

    signal activityLaunchRequested(string activityId, int level, var stepContext)
    signal logoutRequested()
    signal assistanceRequested(string message)

    // Background gradient banner
    Rectangle {
        id: headerBanner
        width: parent.width
        height: 110 * ApplicationInfo.ratio
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#2563eb" }
            GradientStop { position: 1.0; color: "#1d4ed8" }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 24 * ApplicationInfo.ratio
            anchors.rightMargin: 24 * ApplicationInfo.ratio
            spacing: 16 * ApplicationInfo.ratio

            // Learner visual identity avatar
            Rectangle {
                Layout.preferredWidth: 64 * ApplicationInfo.ratio
                Layout.preferredHeight: 64 * ApplicationInfo.ratio
                radius: 32 * ApplicationInfo.ratio
                color: "#ffffff"
                border.color: "#93c5fd"
                border.width: 3 * ApplicationInfo.ratio

                Text {
                    anchors.centerIn: parent
                    text: {
                        var sym = (deskData && deskData.visual_symbol_key) ? deskData.visual_symbol_key : "";
                        if (sym === "star") return "⭐";
                        if (sym === "rocket") return "🚀";
                        if (sym === "dolphin") return "🐬";
                        if (sym === "lion") return "🦁";
                        if (sym === "sun") return "☀️";
                        if (sym === "owl") return "🦉";
                        return "🎓";
                    }
                    font.pixelSize: 32 * ApplicationInfo.ratio
                }
            }

            // Learner Welcome & Class
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4 * ApplicationInfo.ratio

                RowLayout {
                    spacing: 8 * ApplicationInfo.ratio
                    Text {
                        text: "Ciao, " + (deskData ? deskData.learner_name : workstationManager.currentLearnerName) + "!"
                        color: "#ffffff"
                        font.pixelSize: 22 * ApplicationInfo.ratio
                        font.bold: true
                    }
                    Rectangle {
                        visible: deskData && deskData.class_name ? true : false
                        color: "#3b82f6"
                        radius: 6 * ApplicationInfo.ratio
                        Layout.preferredHeight: 24 * ApplicationInfo.ratio
                        Layout.preferredWidth: classText.implicitWidth + 16 * ApplicationInfo.ratio
                        Text {
                            id: classText
                            anchors.centerIn: parent
                            text: deskData && deskData.class_name ? deskData.class_name : ""
                            color: "#ffffff"
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            font.bold: true
                        }
                    }
                }

                Text {
                    text: {
                        if (deskData && deskData.active_teaching_session_title) {
                            return "📚 Lezione attiva: " + deskData.active_teaching_session_title;
                        }
                        return "Scrivania didattica personalizzata • Kids4Edu Native";
                    }
                    color: "#bfdbfe"
                    font.pixelSize: 13 * ApplicationInfo.ratio
                }
            }

            // Status Badges & Action Buttons
            RowLayout {
                spacing: 12 * ApplicationInfo.ratio

                // Network Status
                Rectangle {
                    color: workstationManager.isOnline ? "#16a34a" : "#ea580c"
                    radius: 8 * ApplicationInfo.ratio
                    Layout.preferredHeight: 32 * ApplicationInfo.ratio
                    Layout.preferredWidth: netLabel.implicitWidth + 20 * ApplicationInfo.ratio
                    Row {
                        anchors.centerIn: parent
                        spacing: 6 * ApplicationInfo.ratio
                        Text {
                            text: workstationManager.isOnline ? "●" : "○"
                            color: "#ffffff"
                            font.pixelSize: 12 * ApplicationInfo.ratio
                        }
                        Text {
                            id: netLabel
                            text: workstationManager.isOnline ? "Online" : "Offline"
                            color: "#ffffff"
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            font.bold: true
                        }
                    }
                }

                // Help Button
                Button {
                    Layout.preferredHeight: 36 * ApplicationInfo.ratio
                    Layout.preferredWidth: 120 * ApplicationInfo.ratio
                    background: Rectangle {
                        color: parent.down ? "#b45309" : "#d97706"
                        radius: 8 * ApplicationInfo.ratio
                    }
                    contentItem: Row {
                        anchors.centerIn: parent
                        spacing: 6 * ApplicationInfo.ratio
                        Text { text: "✋"; font.pixelSize: 14 * ApplicationInfo.ratio }
                        Text {
                            text: "Chiedi Aiuto"
                            color: "#ffffff"
                            font.pixelSize: 12 * ApplicationInfo.ratio
                            font.bold: true
                        }
                    }
                    onClicked: {
                        workstationManager.requestAssistance("Richiesta d'aiuto dalla scrivania didattica");
                        statusNotice = "Richiesta d'aiuto inviata al docente.";
                    }
                }

                // Logout / Change User Button
                Button {
                    Layout.preferredHeight: 36 * ApplicationInfo.ratio
                    Layout.preferredWidth: 90 * ApplicationInfo.ratio
                    background: Rectangle {
                        color: parent.down ? "#991b1b" : "#dc2626"
                        radius: 8 * ApplicationInfo.ratio
                    }
                    contentItem: Text {
                        anchors.centerIn: parent
                        text: "Esci"
                        color: "#ffffff"
                        font.pixelSize: 13 * ApplicationInfo.ratio
                        font.bold: true
                    }
                    onClicked: {
                        studentDeskDialog.visible = false;
                        studentDeskDialog.logoutRequested();
                    }
                }
            }
        }
    }

    // Notice toast
    Rectangle {
        id: noticeBox
        visible: statusNotice.length > 0
        anchors.top: headerBanner.bottom
        anchors.topMargin: 8 * ApplicationInfo.ratio
        anchors.horizontalCenter: parent.horizontalCenter
        width: Math.min(parent.width * 0.8, 500 * ApplicationInfo.ratio)
        height: 36 * ApplicationInfo.ratio
        radius: 8 * ApplicationInfo.ratio
        color: "#fef3c7"
        border.color: "#f59e0b"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: 8 * ApplicationInfo.ratio
            Text {
                text: statusNotice
                color: "#92400e"
                font.pixelSize: 12 * ApplicationInfo.ratio
                font.bold: true
                Layout.fillWidth: true
            }
            Text {
                text: "✕"
                color: "#b45309"
                font.pixelSize: 14 * ApplicationInfo.ratio
                MouseArea {
                    anchors.fill: parent
                    onClicked: statusNotice = ""
                }
            }
        }
    }

    // Scrollable Learning Paths Content
    ScrollView {
        id: deskScroll
        anchors.top: noticeBox.visible ? noticeBox.bottom : headerBanner.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20 * ApplicationInfo.ratio
        clip: true

        ColumnLayout {
            width: deskScroll.width
            spacing: 24 * ApplicationInfo.ratio

            // Section 1: Assigned Learning Paths
            Text {
                text: "🎯 I Tuoi Percorsi Didattici Assegnati"
                font.pixelSize: 18 * ApplicationInfo.ratio
                font.bold: true
                color: "#0f172a"
            }

            // Path Cards Repeater
            Repeater {
                model: (deskData && deskData.assigned_paths) ? deskData.assigned_paths : []

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: pathCol.implicitHeight + 32 * ApplicationInfo.ratio
                    radius: 14 * ApplicationInfo.ratio
                    color: "#ffffff"
                    border.color: "#e2e8f0"
                    border.width: 1

                    ColumnLayout {
                        id: pathCol
                        anchors.fill: parent
                        anchors.margins: 16 * ApplicationInfo.ratio
                        spacing: 12 * ApplicationInfo.ratio

                        // Path Header
                        RowLayout {
                            Layout.fillWidth: true
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2 * ApplicationInfo.ratio
                                RowLayout {
                                    spacing: 8 * ApplicationInfo.ratio
                                    Rectangle {
                                        color: "#f1f5f9"
                                        radius: 4 * ApplicationInfo.ratio
                                        Layout.preferredHeight: 20 * ApplicationInfo.ratio
                                        Layout.preferredWidth: subjText.implicitWidth + 12 * ApplicationInfo.ratio
                                        Text {
                                            id: subjText
                                            anchors.centerIn: parent
                                            text: modelData.subject.toUpperCase()
                                            color: "#475569"
                                            font.pixelSize: 10 * ApplicationInfo.ratio
                                            font.bold: true
                                        }
                                    }
                                    Text {
                                        text: "Difficoltà: Livello " + modelData.difficulty_level
                                        color: "#64748b"
                                        font.pixelSize: 11 * ApplicationInfo.ratio
                                    }
                                }
                                Text {
                                    text: modelData.title
                                    font.pixelSize: 16 * ApplicationInfo.ratio
                                    font.bold: true
                                    color: "#1e293b"
                                }
                                Text {
                                    text: modelData.description || ""
                                    font.pixelSize: 12 * ApplicationInfo.ratio
                                    color: "#64748b"
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                }
                            }

                            // Progress percentage pill
                            Rectangle {
                                color: "#eff6ff"
                                radius: 8 * ApplicationInfo.ratio
                                Layout.preferredHeight: 36 * ApplicationInfo.ratio
                                Layout.preferredWidth: 70 * ApplicationInfo.ratio
                                Text {
                                    anchors.centerIn: parent
                                    text: modelData.steps_total > 0 ?
                                          Math.round((modelData.steps_completed / modelData.steps_total) * 100) + "%" : "0%"
                                    color: "#2563eb"
                                    font.pixelSize: 14 * ApplicationInfo.ratio
                                    font.bold: true
                                }
                            }
                        }

                        // Steps Timeline Grid
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12 * ApplicationInfo.ratio

                            Repeater {
                                model: modelData.steps || []

                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 90 * ApplicationInfo.ratio
                                    radius: 10 * ApplicationInfo.ratio
                                    color: {
                                        if (modelData.status === "completed") return "#f0fdf4";
                                        if (modelData.status === "in_progress") return "#eff6ff";
                                        return "#f8fafc";
                                    }
                                    border.color: {
                                        if (modelData.status === "completed") return "#86efac";
                                        if (modelData.status === "in_progress") return "#93c5fd";
                                        return "#e2e8f0";
                                    }
                                    border.width: modelData.status === "in_progress" ? 2 : 1

                                    ColumnLayout {
                                        anchors.fill: parent
                                        anchors.margins: 10 * ApplicationInfo.ratio
                                        spacing: 4 * ApplicationInfo.ratio

                                        RowLayout {
                                            Layout.fillWidth: true
                                            Text {
                                                text: (modelData.status === "completed") ? "✓" : (index + 1)
                                                font.pixelSize: 12 * ApplicationInfo.ratio
                                                font.bold: true
                                                color: (modelData.status === "completed") ? "#16a34a" : "#64748b"
                                            }
                                            Text {
                                                text: modelData.title || modelData.activity_id
                                                font.pixelSize: 13 * ApplicationInfo.ratio
                                                font.bold: true
                                                color: "#1e293b"
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }
                                        }

                                        Text {
                                            text: modelData.instructions || ("Attività: " + modelData.activity_id)
                                            font.pixelSize: 11 * ApplicationInfo.ratio
                                            color: "#64748b"
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }

                                        Button {
                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 28 * ApplicationInfo.ratio
                                            enabled: modelData.status !== "locked"
                                            background: Rectangle {
                                                color: {
                                                    if (modelData.status === "completed") return "#16a34a";
                                                    if (modelData.status === "in_progress") return "#2563eb";
                                                    return "#94a3b8";
                                                }
                                                radius: 6 * ApplicationInfo.ratio
                                            }
                                            contentItem: Text {
                                                anchors.centerIn: parent
                                                text: {
                                                    if (modelData.status === "completed") return "Rigioca";
                                                    if (modelData.status === "in_progress") return "Gioca Ora ▶";
                                                    return "Bloccato 🔒";
                                                }
                                                color: "#ffffff"
                                                font.pixelSize: 11 * ApplicationInfo.ratio
                                                font.bold: true
                                            }
                                            onClicked: {
                                                studentDeskDialog.visible = false;
                                                studentDeskDialog.activityLaunchRequested(
                                                    modelData.activity_id,
                                                    modelData.required_level_min || 1,
                                                    {
                                                        "assignment_id": parent.parent.parent.parent.parent.modelData.assignment_id,
                                                        "step_id": modelData.step_id,
                                                        "completion_criteria": modelData.completion_criteria
                                                    }
                                                );
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Empty state if no paths assigned
            Rectangle {
                visible: !deskData || !deskData.assigned_paths || deskData.assigned_paths.length === 0
                Layout.fillWidth: true
                Layout.preferredHeight: 120 * ApplicationInfo.ratio
                radius: 12 * ApplicationInfo.ratio
                color: "#ffffff"
                border.color: "#e2e8f0"
                Text {
                    anchors.centerIn: parent
                    text: "Nessun percorso didattico assegnato al momento.\nPuoi allenarti liberamente con le attività di seguito!"
                    horizontalAlignment: Text.AlignHCenter
                    color: "#64748b"
                    font.pixelSize: 14 * ApplicationInfo.ratio
                }
            }

            // Section 2: Free Play Activities
            Text {
                text: "🎲 Attività Libere Disponibili"
                font.pixelSize: 18 * ApplicationInfo.ratio
                font.bold: true
                color: "#0f172a"
            }

            // Free Play Activities Grid
            GridLayout {
                Layout.fillWidth: true
                columns: Math.max(2, Math.floor(deskScroll.width / (220 * ApplicationInfo.ratio)))
                rowSpacing: 14 * ApplicationInfo.ratio
                columnSpacing: 14 * ApplicationInfo.ratio

                Repeater {
                    model: (deskData && deskData.free_play_activities) ? deskData.free_play_activities : [
                        { "activity_id": "colors", "name": "Colori", "discipline": "logica", "difficulty": 1 },
                        { "activity_id": "babymatch", "name": "Elementi corrispondenti", "discipline": "forme", "difficulty": 1 },
                        { "activity_id": "adjacent_numbers", "name": "Numeri adiacenti", "discipline": "matematica", "difficulty": 2 },
                        { "activity_id": "ballcatch", "name": "Palla verso Tux", "discipline": "coordinazione", "difficulty": 1 },
                        { "activity_id": "memory", "name": "Memoria Immagini", "discipline": "memoria", "difficulty": 1 }
                    ]

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 110 * ApplicationInfo.ratio
                        radius: 10 * ApplicationInfo.ratio
                        color: "#ffffff"
                        border.color: "#cbd5e1"
                        border.width: 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12 * ApplicationInfo.ratio
                            spacing: 6 * ApplicationInfo.ratio

                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    text: modelData.name || modelData.activity_id
                                    font.pixelSize: 14 * ApplicationInfo.ratio
                                    font.bold: true
                                    color: "#1e293b"
                                    Layout.fillWidth: true
                                    elide: Text.ElideRight
                                }
                                Text {
                                    text: "★ " + (modelData.difficulty || 1)
                                    color: "#eab308"
                                    font.pixelSize: 12 * ApplicationInfo.ratio
                                    font.bold: true
                                }
                            }

                            Text {
                                text: "ID: " + modelData.activity_id
                                color: "#64748b"
                                font.pixelSize: 11 * ApplicationInfo.ratio
                            }

                            Button {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 28 * ApplicationInfo.ratio
                                background: Rectangle {
                                    color: parent.down ? "#1d4ed8" : "#2563eb"
                                    radius: 6 * ApplicationInfo.ratio
                                }
                                contentItem: Text {
                                    anchors.centerIn: parent
                                    text: "Gioca Libero ▶"
                                    color: "#ffffff"
                                    font.pixelSize: 11 * ApplicationInfo.ratio
                                    font.bold: true
                                }
                                onClicked: {
                                    studentDeskDialog.visible = false;
                                    studentDeskDialog.activityLaunchRequested(modelData.activity_id, 1, null);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Refresh desk data on visibility change or learner change
    onVisibleChanged: {
        if (visible && workstationManager.currentLearnerId.length > 0) {
            refreshDesk();
        }
    }

    function refreshDesk() {
        var raw = workstationManager.getStudentDeskData();
        if (raw && Object.keys(raw).length > 0) {
            deskData = raw;
        }
    }

    Connections {
        target: workstationManager
        function onStudentDeskDataReady(data) {
            deskData = data;
        }
        function onLearnerChanged() {
            if (studentDeskDialog.visible) {
                refreshDesk();
            }
        }
    }
}
