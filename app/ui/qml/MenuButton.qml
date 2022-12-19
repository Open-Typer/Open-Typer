/*
 * MenuButton.qml
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

CustomToolButton {
	property CustomMenu menu: CustomMenu {}
	property bool ignoreClosing: false
	id: control
	font.pointSize: 10
	onClicked: {
		if(menu.visible)
		{
			ignoreClosing = true;
			menu.close();
		}
		else
			menu.open();
	}
	implicitWidth: metrics.boundingRect(text).width + 20
	implicitHeight: metrics.height + 16

	Label {
		visible: menu.isSubMenu
		anchors.fill: parent
		text: ">"
		horizontalAlignment: Qt.AlignRight
		verticalAlignment: Qt.AlignVCenter
		padding: 10
	}

	FontMetrics {
		id: metrics
		font: control.font
	}
}
