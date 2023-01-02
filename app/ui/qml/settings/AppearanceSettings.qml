/*
 * AppearanceSettings.qml
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
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0 as Platform
import OpenTyper 1.0
import ".."

ColumnLayout {
	property bool simple: false
	//: Set custom color
	readonly property string customColorStr: qsTr("Set custom")
	function setLightTheme() {
		ThemeEngine.setSimpleTheme(ThemeEngine.LightTheme);
	}

	function setDarkTheme() {
		ThemeEngine.setSimpleTheme(ThemeEngine.DarkTheme);
	}

	Platform.ColorDialog {
		id: exerciseTextColorDialog
		modality: Qt.WindowModal
		currentColor: ThemeEngine.exerciseTextColor
		onColorChanged: ThemeEngine.exerciseTextColor = color
		Component.onCompleted: updateColor()
		function updateColor() { color = ThemeEngine.exerciseTextColor; }
	}

	Platform.ColorDialog {
		id: inputTextColorDialog
		modality: Qt.WindowModal
		currentColor: ThemeEngine.inputTextColor
		onColorChanged: ThemeEngine.inputTextColor = color
		Component.onCompleted: updateColor()
		function updateColor() { color = ThemeEngine.inputTextColor; }
	}

	Platform.ColorDialog {
		id: bgColorDialog
		modality: Qt.WindowModal
		currentColor: ThemeEngine.bgColor
		onColorChanged: ThemeEngine.bgColor = color
		Component.onCompleted: updateColor()
		function updateColor() { color = ThemeEngine.bgColor; }
	}

	Platform.ColorDialog {
		id: paperColorDialog
		modality: Qt.WindowModal
		currentColor: ThemeEngine.paperColor
		onColorChanged: ThemeEngine.paperColor = color
		Component.onCompleted: updateColor()
		function updateColor() { color = ThemeEngine.paperColor; }
	}

	Item {
		// Connections didn't work here for some reason...
		property int theme: ThemeEngine.theme
		onThemeChanged: {
			exerciseTextColorDialog.updateColor();
			inputTextColorDialog.updateColor();
			bgColorDialog.updateColor();
			paperColorDialog.updateColor();
		}
	}

	// Theme section
	Label {
		text: qsTr("Themes")
		font.bold: true
	}

	RowLayout {
		RadioButton {
			//: The light theme
			text: qsTr("Light")
			checked: ThemeEngine.style === ThemeEngine.LightStyle
			onPressed: setLightTheme()
		}

		RadioButton {
			//: The dark theme
			text: qsTr("Dark")
			checked: ThemeEngine.style === ThemeEngine.DarkStyle
			onPressed: setDarkTheme()
		}
	}

	MenuSeparator { Layout.fillWidth: true }

	// Text section
	Label {
		text: qsTr("Text")
		font.bold: true
	}

	RowLayout {
		Label {
			text: qsTr("Font:")
		}
		FontComboBox {
			fixedPitch: true
			Component.onCompleted: {
				family = ThemeEngine.fontFamily
				// This has to be connected after getting font family from settings
				onFamilyChanged.connect(function() { ThemeEngine.fontFamily = family; });
			}
		}
	}

	RowLayout {
		visible: !simple
		Label {
			text: qsTr("Exercise text color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: exerciseTextColorDialog.open()
		}

		ColorButton {
			color: ThemeEngine.defaultExerciseTextColor()
			checked: ThemeEngine.exerciseTextColor == color
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.exerciseTextColor = color
		}

		ColorButton {
			color: ThemeEngine.style === ThemeEngine.DarkStyle ? Qt.rgba(1, 1, 1, 1) : Qt.rgba(0, 0, 0, 1)
			checked: ThemeEngine.exerciseTextColor == color
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.exerciseTextColor = color
		}
	}

	RowLayout {
		visible: !simple
		Label {
			text: qsTr("Input text color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: inputTextColorDialog.open()
		}

		ColorButton {
			color: ThemeEngine.defaultInputTextColor(ThemeEngine.style === ThemeEngine.DarkStyle)
			checked: ThemeEngine.inputTextColor == color
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.inputTextColor = color
		}
	}

	MenuSeparator { visible: !simple; Layout.fillWidth: true }

	// Background section
	Label {
		visible: !simple
		text: qsTr("Background")
		font.bold: true
	}

	RowLayout {
		visible: !simple
		Label {
			text: qsTr("Background color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: bgColorDialog.open()
		}

		ColorButton {
			color: ThemeEngine.defaultBgColor(ThemeEngine.style === ThemeEngine.DarkStyle)
			checked: ThemeEngine.bgColor == color
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.bgColor = color
		}
	}

	MenuSeparator { visible: !simple; Layout.fillWidth: true }

	// Paper section
	Label {
		visible: !simple
		text: qsTr("Paper")
		font.bold: true
	}

	RowLayout {
		visible: !simple
		Label {
			text: qsTr("Paper color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: paperColorDialog.open()
		}

		ColorButton {
			color: ThemeEngine.defaultPaperColor(ThemeEngine.style === ThemeEngine.DarkStyle)
			checked: ThemeEngine.paperColor == color
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.paperColor = color
		}
	}
}
