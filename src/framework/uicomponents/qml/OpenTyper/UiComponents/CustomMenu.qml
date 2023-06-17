/*
 * CustomMenu.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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
import OpenTyper.Ui 1.0

Menu {
	property bool isSubMenu: false
	function updateWidth() {
		let maxWidth = 0;
		for(let i = 0; i < count; i++)
			maxWidth = Math.max(maxWidth, itemAt(i).implicitWidth);
		background.implicitWidth = maxWidth;
	}
	x: isSubMenu ? parent.width : 0
	y: isSubMenu ? 0 : parent.height
	font.pointSize: 10
	background: Rectangle {
		color: ThemeEngine.bgColor;
		border.color: ThemeEngine.borderColor
		radius: 10
		implicitHeight: 40
	}
	delegate: CustomMenuItem {}
	onAboutToShow: updateWidth()
}
