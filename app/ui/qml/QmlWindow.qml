/*
 * QmlWindow.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.15
import OpenTyper 1.0

ApplicationWindow {
	Material.theme: ThemeEngine.style === ThemeEngine.DarkStyle ? Material.Dark : Material.Light
	Material.accent: Material.LightBlue // TODO: Use accent color (maybe from ThemeEngine)
	// TODO: Load window geometry from settings
	width: 1200
	height: 800
	visible: true
	id: root

	ColumnLayout {
		id: mainLayout
		anchors.fill: parent
		spacing: 0
		Panel {
			Layout.fillWidth: true
			control: RowLayout {
				CustomToolButton {
					objectName: "settingsButton"
					icon.name: "settings"
					toolTipText: qsTr("Options")
				}
				CustomToolButton {
					objectName: "openButton"
					icon.name: "open"
					toolTipText: qsTr("Open")
				}
				CustomToolButton {
					objectName: "printButton"
					icon.name: "print"
					toolTipText: qsTr("Print")
				}
				ToolSeparator {}
				CustomToolButton {
					objectName: "typingTestButton"
					icon.name: "paper"
					text: qsTr("Typing test")
				}
				CustomToolButton {
					objectName: "timedExButton"
					icon.name: "time"
					text: qsTr("Timed exercise")
				}
				CustomToolButton {
					objectName: "errorWordsButton"
					icon.name: "close"
					text: qsTr("Text from error words")
				}
				CustomToolButton {
					objectName: "reverseTextButton"
					icon.name: "rewind"
					text: qsTr("Reverse text")
				}
			}
		}
		Panel {
			Layout.fillWidth: true
			control: RowLayout {
				CustomToolButton {
					objectName: "repeatExButton"
					icon.name: "repeat"
					toolTipText: qsTr("Repeat exercise")
				}
				CustomToolButton {
					objectName: "closeLoadedExButton"
					icon.name: "close"
					toolTipText: qsTr("Close loaded exercise")
				}
				CustomToolButton {
					objectName: "previousExButton"
					icon.name: "left"
					toolTipText: qsTr("Previous exercise")
				}
				CustomToolButton {
					objectName: "nextExButton"
					icon.name: "right"
					toolTipText: qsTr("Next exercise")
				}
				CustomComboBox { objectName: "lessonBox" }
				CustomComboBox { objectName: "sublessonBox" }
				CustomComboBox { objectName: "exerciseBox" }
				ToolSeparator {}
				CustomToolButton {
					objectName: "statsButton"
					icon.name: "stats"
					text: qsTr("Exercise history")
				}
			}
		}
		Paper {
			Layout.fillHeight: true
			Layout.topMargin: 10
			Layout.alignment: Qt.AlignHCenter
		}
	}
}
