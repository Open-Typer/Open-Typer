/*
 * BehaviorSettings.qml
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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import OpenTyper 1.0

ColumnLayout {
	Label {
		text: qsTr("Exercise validation")
		font.bold: true
		font.pointSize: 12
	}
	RowLayout {
		Layout.fillWidth: true
		Label {
			text: qsTr("Error penalty:")
			Layout.fillWidth: true
		}
		SpinBox {
			from: 0
			to: 100
			value: Settings.errorPenalty()
			onValueChanged: Settings.setErrorPenalty(value)
		}
	}
	CheckBox {
		id: mistakeLimitCheckBox
		text: qsTr("Limit number of mistakes")
		checked: Settings.mistakeLimit()
		onCheckedChanged: Settings.setMistakeLimit(checked)
	}
	RowLayout {
		enabled: mistakeLimitCheckBox.checked
		Layout.fillWidth: true
		Label {
			text: qsTr("Number of characters in one word with max. 1 mistake:")
			Layout.fillWidth: true
		}
		SpinBox {
			from: 1
			to: 100
			value: Settings.mistakeChars()
			onValueChanged: Settings.setMistakeChars(value)
		}
	}
	Label {
		visible: QmlUtils.osWindows()
		text: qsTr("Updates")
		font.bold: true
		font.pointSize: 12
	}
	CheckBox {
		visible: QmlUtils.osWindows()
		text: qsTr("Check for updates on startup")
		checked: Settings.updateChecks()
		onCheckedChanged: Settings.setUpdateChecks(checked)
	}
}
