/*
 * CustomComboBox.qml
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
import QtQuick.Window 2.12
import OpenTyper 1.0

// An auto-resizing ComboBox (based on its items)
// Fixes: material style popup background color
// Elevation (shadow) is disabled
ComboBox {
	readonly property color accent: Material.accent
	id: control
	FontMetrics {
		id: fontMetric
		font: control.contentItem.font
	}
	Binding on implicitWidth {
		value: computeWidth(model)
	}
	Material.background: Qt.rgba(accent.r, accent.g, accent.b, 0.3);
	background.layer.enabled: false

	popup.background: Rectangle {
		color: ThemeEngine.bgColor
		border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
		radius: 10
	}
	popup.implicitWidth: computeWidth(model)
	popup.height: Math.min(popup.contentItem.implicitHeight, Window.height - popup.topMargin - popup.bottomMargin - mapToItem(Window.contentItem, x, y).y - height * 2)
	popup.y: height
	background.clip: true
	function computeWidth(mdl) {
		if (mdl === null || mdl === undefined)
			return 100; // width of an empty combo box
		var longest = "";
		for (var i = 0; i < mdl.length; i++) {
			var txt = mdl[i];
			if (txt.length > longest.length)
				longest = txt;
		}
		var pwidth = fontMetric.boundingRect(longest).width;
		pwidth += control.contentItem.rightPadding + control.contentItem.leftPadding;
		pwidth += control.indicator.width
		return pwidth;
	}
}
