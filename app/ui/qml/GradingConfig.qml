/*
 * GradeConfig.qml
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

ColumnLayout {
	property Class currentClass: null

	RowLayout {
		id: nameLayout
		visible: currentClass != null
		Layout.fillWidth: true

		Label {
			//: Class name
			text: qsTr("Name:")
		}

		TextField {
			text: nameLayout.visible ? currentClass.name : ""
			onTextChanged: nameLayout.visible ? currentClass.name = text : {}
			Layout.fillWidth: true
		}
	}

	Label {
		text: qsTr("Description:")
		visible: currentClass != null
	}

	TextArea {
		property int maxLength: 64
		placeholderText: qsTr("Enter description")
		text: nameLayout.visible ? currentClass.description : ""
		visible: nameLayout.visible
		padding: 10
		onTextChanged: {
			if(nameLayout.visible)
				currentClass.description = text;
			if(length > maxLength)
				remove(maxLength, length);
		}
		Layout.fillWidth: true
		background: Rectangle {
			implicitWidth: 200
			implicitHeight: 40
			color: "transparent"
			border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
			radius: 10
		}
	}

	MenuSeparator { Layout.fillWidth: true; visible: currentClass != null }

	Label {
		text: qsTr("Target number of hits per minute")
		font.pointSize: 12
		font.bold: true
	}

	SpinBox {
		from: 25
		to: 750
		value: currentClass == null ? Settings.targetHitsPerMinute() : currentClass.targetHitsPerMinute
		onValueChanged: currentClass == null ? Settings.setTargetHitsPerMinute(value) : currentClass.targetHitsPerMinute = value
	}

	MenuSeparator { Layout.fillWidth: true; visible: currentClass == null }

	Label {
		//: Type of something
		text: qsTr("Type")
		font.pointSize: 12
		font.bold: true
		visible: currentClass == null
	}

	RowLayout {
		visible: currentClass == null

		RadioButton {
			text: qsTr("Net hits")
			checked: Settings.gradeNetHits()
			onCheckedChanged: Settings.setGradeNetHits(checked)
		}

		RadioButton {
			text: qsTr("Gross hits")
			checked: !Settings.gradeNetHits()
		}
	}
}
