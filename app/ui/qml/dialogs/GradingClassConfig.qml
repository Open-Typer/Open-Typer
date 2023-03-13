/*
 * GradingClassConfig.qml
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
import OpenTyper 1.0
import "../controls"
import "../core"
import ".."

CustomDialog {
	windowTitle: qsTr("Classes")
	draggable: false
	fillWindow: true
	standardButtons: Dialog.Close

	contentComponent: StackView {
		id: stackView
		initialItem: listComponent

		Component {
			id: listComponent

			ColumnLayout {
				readonly property Class currentClass: ClassManager.classes[listView.currentIndex]

				Label {
					text: windowTitle
					font.pointSize: 12
					font.bold: true
				}

				ListView {
					id: listView
					Layout.fillWidth: true
					Layout.fillHeight: true
					spacing: 15
					model: ClassManager.classes
					clip: true
					delegate: ListButton {
						width: listView.width
						title: model.name
						text: model.description
						onClicked: stackView.push(configComponent)
					}
				}
			}
		}

		Component {
			id: configComponent

			ColumnLayout {
				AccentButton {
					icon.name: "left"
					onClicked: stackView.pop()
					Accessible.name: QmlUtils.translateStandardButton("Close")
				}

				CustomFlickable {
					Layout.fillWidth: true
					Layout.fillHeight: true
					contentWidth: contentItem.childrenRect.width
					contentHeight: contentItem.childrenRect.height
					flickableDirection: Flickable.AutoFlickIfNeeded
					showVerticalScrollBar: true
					clip: true

					GradingConfig {
						currentClass: stackView.get(0).currentClass
					}
				}
			}
		}
	}
}
