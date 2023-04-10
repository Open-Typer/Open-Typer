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

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.5
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1 as Platform
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0
import OpenTyper.Global 1.0

ColumnLayout {
	property bool simple: false
	//: Set custom color
	readonly property string customColorStr: qsTr("Set custom")
	function setLightTheme() {
		var exTextAccentChecked = exTextColorAccent.checked;
		ThemeEngine.theme = ThemeEngine.LightTheme;
		ThemeEngine.exerciseTextColor = ThemeEngine.defaultExerciseTextColor(ThemeEngine.currentAccentColor);
	}

	function setDarkTheme() {
		var exTextAccentChecked = exTextColorAccent.checked;
		ThemeEngine.theme = ThemeEngine.DarkTheme;
		ThemeEngine.exerciseTextColor = ThemeEngine.defaultExerciseTextColor(ThemeEngine.currentAccentColor);
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
		id: themesLabel
		text: qsTr("Themes")
		font.bold: true
		font.pointSize: 12
	}

	GridLayout {
		columns: 2

		RadioButton {
			//: The light theme
			text: qsTr("Light")
			checked: ThemeEngine.theme === ThemeEngine.LightTheme
			onPressed: setLightTheme()
			Accessible.name: themesLabel.text + " " + text
		}

		RadioButton {
			//: The dark theme
			text: qsTr("Dark")
			checked: ThemeEngine.theme === ThemeEngine.DarkTheme
			onPressed: setDarkTheme()
		}

		Label {
			text: qsTr("Accent color:")
		}

		RowLayout {
			Repeater {
				model: [ThemeEngine.Accent_Red, ThemeEngine.Accent_Orange, ThemeEngine.Accent_Yellow,
					ThemeEngine.Accent_Green, ThemeEngine.Accent_LightBlue,
					ThemeEngine.Accent_Purple, ThemeEngine.Accent_Pink]
				ColorButton {
					readonly property int accentColor: modelData
					color: {
						// "use" ThemeEngine.theme so that this property reevaluates when it changes
						if(ThemeEngine.theme == ThemeEngine.DarkTheme);
						return ThemeEngine.getAccentColor(accentColor);
					}
					checked: ThemeEngine.accentColor == accentColor
					autoExclusive: true
					checkable: true
					onPressed: {
						if(exTextColorAccent.checked)
							ThemeEngine.exerciseTextColor = ThemeEngine.defaultExerciseTextColor(color);
						ThemeEngine.accentColor = accentColor;
					}
					//: %1 is the number of current color, for example "Accent color 2"
					Accessible.name: qsTr("Accent color %1").arg(index + 1)
				}
			}
		}
	}

	MenuSeparator { Layout.fillWidth: true }

	// Text section
	Label {
		text: qsTr("Text")
		font.bold: true
		font.pointSize: 12
	}

	RowLayout {
		Label {
			id: fontLabel
			text: qsTr("Font:")
		}
		FontComboBox {
			fixedPitch: true
			Accessible.name: fontLabel.text
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
			id: exTextColorLabel
			text: qsTr("Exercise text color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: exerciseTextColorDialog.open()
			Accessible.name: exTextColorLabel.text + " " + text
		}

		ColorButton {
			property bool wasChecked: checked
			id: exTextColorAccent
			color: ThemeEngine.defaultExerciseTextColor(ThemeEngine.currentAccentColor)
			// For some reason checking for color equality here works only after converting the colors to strings
			// (this happens only after using Qt.darker() with any of the colors)
			checked: ThemeEngine.exerciseTextColor + "" == color + ""
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.exerciseTextColor = color
		}

		ColorButton {
			color: ThemeEngine.theme === ThemeEngine.DarkTheme ? Qt.rgba(1, 1, 1, 1) : Qt.rgba(0, 0, 0, 1)
			checked: ThemeEngine.exerciseTextColor == color
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.exerciseTextColor = color
		}
	}

	RowLayout {
		visible: !simple
		Label {
			id: inTextColorLabel
			text: qsTr("Input text color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: inputTextColorDialog.open()
			Accessible.name: inTextColorLabel.text + " " + text
		}

		ColorButton {
			color: ThemeEngine.defaultInputTextColor(ThemeEngine.theme === ThemeEngine.DarkTheme)
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
		font.pointSize: 12
	}

	RowLayout {
		visible: !simple
		Label {
			id: bgColorLabel
			text: qsTr("Background color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: bgColorDialog.open()
			Accessible.name: bgColorLabel.text + " " + text
		}

		ColorButton {
			color: ThemeEngine.defaultBgColor(ThemeEngine.theme === ThemeEngine.DarkTheme)
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
		font.pointSize: 12
	}

	RowLayout {
		visible: !simple
		Label {
			id: paperColorLabel
			text: qsTr("Paper color:")
		}

		Button {
			visible: !QmlUtils.osWasm()
			text: customColorStr
			font.capitalization: Font.MixedCase
			onClicked: paperColorDialog.open()
			Accessible.name: paperColorLabel.text + " " + text
		}

		ColorButton {
			color: ThemeEngine.defaultPaperColor(ThemeEngine.theme === ThemeEngine.DarkTheme)
			checked: ThemeEngine.paperColor == color
			autoExclusive: true
			checkable: true
			onPressed: ThemeEngine.paperColor = color
		}
	}

	MenuSeparator { visible: !simple; Layout.fillWidth: true }

	// Keyboard section
	Label {
		visible: !simple
		text: qsTr("Keyboard")
		font.bold: true
		font.pointSize: 12
	}

	CheckBox {
		visible: !simple
		text: qsTr("Use key colors based on fingers")
		checked: Settings.getValue("keyboard", "keyboardFingerColors")
		onCheckedChanged: {
			Settings.setValue("keyboard", "keyboardFingerColors", checked);
			QmlUtils.screenKeyboardChanged(false);
		}
	}
}
