/*
 * ColorButton.qml
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
import QtQuick.Controls.Material 2.5

RoundButton {
	property color color: Material.background
	id: control
	onColorChanged: Material.background = color
	Rectangle {
		width: control.background.width
		height: control.background.height
		anchors.centerIn: parent
		radius: width / 2
		color: Qt.rgba(0, 0, 0, 0)
		border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
	}
	Rectangle {
		visible: control.checked
		width: control.background.width + 6
		height: control.background.height + 6
		anchors.centerIn: parent
		radius: width / 2
		color: Qt.rgba(0, 0, 0, 0)
		border.color: Material.theme == Material.Dark ? Qt.rgba(1, 1, 1, 1) : Qt.rgba(0, 0, 0, 1)
	}
}
