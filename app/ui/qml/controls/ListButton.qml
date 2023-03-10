/*
 * ListButton.qml
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
import OpenTyper 1.0

ItemDelegate {
	property url iconUrl: ""
	property string title: ""
	id: control

	Rectangle {
		id: borderRect
		anchors.fill: parent
		color: "transparent"
		radius: 10
		border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
	}

	contentItem: RowLayout {
		spacing: -50 // some weird Material style property is probably causing this...

		Image {
			source: iconUrl
			sourceSize.width: 40
			sourceSize.height: 40
		}

		ColumnLayout {
			Layout.fillWidth: true

			Label {
				text: control.title
				font.bold: true
				font.pointSize: 14
			}

			Label {
				text: control.text
			}
		}

		Label {
			text: "❯"
		}
	}
}
