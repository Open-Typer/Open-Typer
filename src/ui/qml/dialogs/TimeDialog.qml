/*
 * TimeDialog.qml
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
import QtQuick.Layouts 1.12
import "../controls"

CustomDialog {
	readonly property var itemStrings: ["1 min", "2 min 30 s", "5 min", "10 min"]
	readonly property var itemValues: [60, 150, 300, 600]
	property int timeSecs: contentItem.timeSecs
	windowTitle: qsTr("Time limit")
	standardButtons: Dialog.Cancel | Dialog.Ok
	contentComponent: ColumnLayout {
		property int timeSecs: timeComboBox.customTime ? minBox.value * 60 + secBox.value : itemValues[timeComboBox.currentIndex]
		CustomComboBox {
			readonly property bool customTime: currentIndex == model.length - 1
			id: timeComboBox
			model: itemStrings.concat([qsTr("Custom")])
			currentIndex: 3 // 10 min by default
			Layout.fillWidth: true
		}
		ColumnLayout {
			visible: timeComboBox.customTime
			Layout.fillHeight: true
			RowLayout {
				Label {
					text: qsTr("Minutes:")
				}
				SpinBox {
					id: minBox
					value: 15
					to: 60
					editable: true
				}
			}
			RowLayout {
				Label {
					text: qsTr("Seconds:")
				}
				SpinBox {
					id: secBox
					value: 0
					to: 59
					editable: true
				}
			}
		}
	}
}
