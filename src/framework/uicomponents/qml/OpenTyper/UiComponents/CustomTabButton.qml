/*
 * CustomTabButton.qml
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

TabButton {
	property bool closable: false
	signal closed()
	id: control
	width: closable ? implicitWidth + closeButton.width - 15 : implicitWidth
	height: metrics.boundingRect.height + 15
	font.capitalization: Font.MixedCase

	contentItem: Text {
		text: control.text
		font: control.font
		color: Material.foreground
		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignVCenter
	}

	TextMetrics {
		id: metrics
		font: control.font
		text: control.text
	}

	RoundToolButton {
		id: closeButton
		visible: closable
		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter
		icon.name: "close"
		icon.height: parent.height / 2
		icon.width: parent.height / 2
		toolTipText: qsTr("Close tab")
		onClicked: closed()
	}
}
