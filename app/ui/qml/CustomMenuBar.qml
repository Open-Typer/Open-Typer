/*
 * CustomMenuBar.qml
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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3

Panel {
	control: RowLayout {
		CustomMenu {
			text: qsTr("&File");
			menu: Menu {
				id: menu
				background: Rectangle {
					color: backgroundColor();
					implicitWidth: 200
					implicitHeight: 40
				}

				MenuItem { text: qsTr("&New..."); font.pointSize: 10; implicitHeight: 36 }
				MenuItem { text: qsTr("&Open..."); font.pointSize: 10; implicitHeight: 36 }
				MenuSeparator { }
				MenuItem { text: qsTr("&Quit"); font.pointSize: 10; implicitHeight: 36; onClicked: Qt.quit(); }
			}
		}
	}

	function backgroundColor() {
		if(Material.theme === Material.Light)
			return Qt.rgba(1, 1, 1, 1);
		else
			return Qt.rgba(0, 0, 0, 1);
	}
}
