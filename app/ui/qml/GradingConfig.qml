/*
 * GradingConfig.qml
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
import "controls"

ColumnLayout {
	property Class currentClass: null
	readonly property var locale: Qt.locale(LanguageManager.languageStr)
	readonly property string alphabet: "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

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
		id: targetHitsBox
		from: 25
		to: 750
		value: currentClass == null ? Settings.targetHitsPerMinute() : currentClass.targetHitsPerMinute
		onValueChanged: currentClass == null ? Settings.setTargetHitsPerMinute(value) : currentClass.targetHitsPerMinute = value
	}

	RadioButton {
		id: numbersButton
		text: qsTr("Use numbers")
		visible: currentClass == null
		checked: Settings.gradingMethod() === "numbers"
		onCheckedChanged: {
			if(checked)
				Settings.setGradingMethod("numbers");
		}
	}

	RadioButton {
		id: lettersButton
		//: Use "characters" would be valid too
		text: qsTr("Use letters")
		visible: currentClass == null
		checked: Settings.gradingMethod() === "letters"
		onCheckedChanged: {
			if(checked)
				Settings.setGradingMethod("letters");
		}
	}

	RowLayout {
		visible: currentClass == null

		Label {
			text: qsTr("Worst grade:")
			Layout.alignment: Qt.AlignVCenter
		}

		SpinBox {
			visible: numbersButton.checked
			from: 0
			to: 100
			value: Settings.gradeStartNumber()
			onValueChanged: Settings.setGradeStartNumber(value)
		}

		CustomComboBox {
			visible: lettersButton.checked
			model: alphabet.split("")
			currentIndex: alphabet.indexOf(Settings.gradeStartLetter())
			onCurrentTextChanged: Settings.setGradeStartLetter(currentText)
		}
	}

	RowLayout {
		visible: currentClass == null

		Label {
			text: qsTr("Best grade:")
			Layout.alignment: Qt.AlignVCenter
		}

		SpinBox {
			visible: numbersButton.checked
			from: 0
			to: 100
			value: Settings.gradeEndNumber()
			onValueChanged: Settings.setGradeEndNumber(value)
		}

		CustomComboBox {
			visible: lettersButton.checked
			model: alphabet.split("")
			currentIndex: alphabet.indexOf(Settings.gradeEndLetter())
			onCurrentTextChanged: Settings.setGradeEndLetter(currentText)
		}
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

	MenuSeparator { Layout.fillWidth: true; visible: currentClass != null }

	Label {
		text: qsTr("Grading based on months")
		font.pointSize: 12
		font.bold: true
		visible: currentClass != null
	}

	Component {
		id: monthConfig

		GridLayout {
			columns: 3

			Repeater {
				model: 12

				Frame {
					Layout.fillWidth: true
					background: Rectangle {
						color: "transparent"
						border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
						radius: 10
					}

					RowLayout {
						anchors.fill: parent

						Label {
							Layout.fillWidth: true
							text: locale.standaloneMonthName(index, Locale.LongFormat)
						}

						SpinBox {
							value: {
								let targetHits = currentClass.targetHitsForMonth(index + 1);
								currentClass.gradeConfig;
								if(targetHits === 0)
									return targetHitsBox.value;
								else
									return targetHits;
							}
							from: targetHitsBox.from
							to: targetHitsBox.to
							editable: true
							up.indicator: null
							down.indicator: null
							implicitWidth: 66
							onValueModified: currentClass.setTargetHitsForMonth(index + 1, value);
						}

						CustomToolButton {
							icon.name: "repeat"
							visible: {
								let targetHits = currentClass.targetHitsForMonth(index + 1);
								currentClass.gradeConfig;
								return (targetHits !== 0);
							}
							//: To reset a value
							toolTipText: qsTr("Reset")
							onClicked: currentClass.setTargetHitsForMonth(index + 1, 0)
						}
					}
				}
			}
		}
	}

	Loader {
		sourceComponent: monthConfig
		active: currentClass != null
	}
}
