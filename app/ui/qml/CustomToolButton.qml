/*
 * CustomToolButton.qml
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
import QtQuick.Controls.Material 2.15

// Similar to ToolButton, but it's always a rectangle
// Double click events are not supported, use the clicked() signal like with a QPushButton
Button {
	readonly property color foregroundColor: Material.theme === Material.Dark ? "white" : "black"
	property string toolTipText
	signal clicked()
	font.capitalization: Font.MixedCase
	Material.background: Qt.rgba(0, 0, 0, 0)
	Material.foreground: foregroundColor
	icon.color: foregroundColor
	onReleased: clicked()
	HoverToolTip { text: toolTipText }
}
