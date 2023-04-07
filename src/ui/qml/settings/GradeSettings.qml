/*
 * GradeSettings.qml
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
import OpenTyper.UiComponents 1.0
import "../dialogs"
import ".."

ColumnLayout {
	Control {} // fixes key navigation

	GradingConfig {}

	MenuSeparator { Layout.fillWidth: true }

	RowLayout {
		visible: classBox.count > 1

		Label {
			id: selectedClassLabel
			text: qsTr("Selected class:")
		}

		CustomComboBox {
			id: classBox
			model: {
				let arr1 = [qsTr("No class selected")];
				let arr2 = ClassManager.classNames;
				return arr1.concat(arr2);
			}
			currentIndex: Settings.selectedClass() + 1
			onCurrentIndexChanged: Settings.setSelectedClass(currentIndex - 1)
			Accessible.name: selectedClassLabel.text
		}
	}

	Button {
		text: qsTr("Configure grading for classes")
		font.capitalization: Font.MixedCase
		onClicked: classConfig.open()
	}

	Item {
		GradingClassConfig {
			id: classConfig
		}
	}
}
