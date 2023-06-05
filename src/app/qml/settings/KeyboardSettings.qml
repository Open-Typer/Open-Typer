/*
 * KeyboardSettings.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import ".."

// This settings page can be used only in SettingsDialog!
RowLayout {
	ColumnLayout {
		Pane {
			implicitWidth: 0
			Label {
				text: qsTr("Keyboard layout")
				font.bold: true
				font.pointSize: 12
			}
		}
		KeyboardLayoutList {
			id: layoutList
			Layout.fillWidth: true
			Layout.fillHeight: true
			KeyNavigation.tab: packList
		}
	}
	ToolSeparator { Layout.fillHeight: true }
	ColumnLayout {
		Pane {
			implicitWidth: 0
			Label {
				text: qsTr("Lesson pack")
				font.bold: true
				font.pointSize: 12
			}
		}
		LessonPackList {
			id: packList
			Layout.fillWidth: true
			Layout.fillHeight: true
			keyboardLayout: layoutList.items[layoutList.currentIndex]
		}
	}
	onActiveFocusChanged: {
		if(activeFocus)
			layoutList.forceActiveFocus(Qt.TabFocusReason);
	}
	Component.onCompleted: {
		// Fill SettingsDialog's Flickable
		implicitWidth = parent.fixedWidth;
		implicitHeight = parent.fixedHeight;
	}
}
