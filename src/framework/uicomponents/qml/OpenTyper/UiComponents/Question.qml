/*
 * Question.qml
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
import QtQuick.Layouts 1.12
import OpenTyper.Ui 1.0

Rectangle {
	readonly property color accent: Material.accent
	property string text: ""
	id: root
	height: rowLayout.implicitHeight
	color: Qt.rgba(accent.r, accent.g, accent.b, 0.15)
	border.color: ThemeEngine.borderColor
	radius: 10
	signal accepted()
	signal rejected()

	RowLayout {
		id: rowLayout
		width: parent.width

		Label {
			Layout.fillWidth: true
			padding: 10
			text: root.text
			font.bold: true
		}

		AccentButtonBox {
			standardButtons: Dialog.Yes | Dialog.No
			font.capitalization: Font.MixedCase
			background: null
			onAccepted: {
				closeAnimation.start()
				root.accepted();
			}
			onRejected: {
				closeAnimation.start()
				root.rejected();
			}
		}
	}

	PropertyAnimation {
		id: closeAnimation
		target: root
		property: "opacity"
		to: 0
		duration: 250
		onStopped: {
			root.visible = false;
			root.opacity = 1;
		}
	}
}
