/*
 * ExerciseSummary.qml
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
import QtQuick.Controls.Material 2.5
import QtQuick.Layouts 1.12
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0
import OpenTyper.Validator 1.0
import OpenTyper.Grades 1.0

Rectangle {
	property int padding: 10
	property int fontPointSize: 14
	property ExerciseValidator validator: null
	property int totalTime
	property int totalHits
	property int netHits
	property int grossHits
	property int mistakes
	property real accuracy
	height: columnLayout.implicitHeight + padding * 2
	radius: 10
	color: ThemeEngine.panelColor
	border.color: Material.theme === Material.Light ? Qt.rgba(0, 0, 0, 0.25) : Qt.rgba(1, 1, 1, 0.25)
	clip: true
	onValidatorChanged: {
		resetClass();
		updateGrade();
	}

	function resetClass() {
		classComboBox.currentIndex = Settings.getValue("grades", "selectedClass") + 1;
	}

	function updateGrade() {
		if(validator == null)
			return;
		gradeCalc.validator = validator;
		gradeCalc.targetHitsPerMinute = ClassManager.targetHitsPerMinute(classComboBox.currentIndex - 1);
	}

	GradeCalculator {
		id: gradeCalc
	}

	ColumnLayout {
		id: columnLayout
		x: padding
		y: padding

		RowLayout {
			Layout.fillWidth: true
			Label {
				text: qsTr("Total time:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			Label {
				text: getTime(totalTime)
				font.pointSize: fontPointSize
				function getTime(time) {
					var minutes = Math.floor(time / 60);
					var seconds = time % 60;
					var out = "";
					if(minutes > 0)
						out += minutes + " min ";
					if((seconds > 0) || (minutes + seconds == 0))
						out += seconds + " s";
					return out;
				}
			}
		}

		RowLayout {
			Layout.fillWidth: true
			Label {
				text: qsTr("Total number of hits:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			Label {
				text: totalHits
				font.pointSize: fontPointSize
			}
		}

		RowLayout {
			Layout.fillWidth: true
			Label {
				text: qsTr("Number of net hits per minute:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			Label {
				text: netHits
				font.pointSize: fontPointSize
			}
		}

		RowLayout {
			Layout.fillWidth: true
			Label {
				text: qsTr("Number of gross hits per minute:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			Label {
				text: grossHits
				font.pointSize: fontPointSize
			}
		}

		RowLayout {
			Layout.fillWidth: true
			Label {
				text: qsTr("Mistakes:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			Label {
				text: mistakes
				font.pointSize: fontPointSize
			}
		}

		RowLayout {
			Layout.fillWidth: true
			Label {
				text: qsTr("Accuracy:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			Label {
				text: Math.floor(accuracy * 10000) / 100 + " %"
				font.pointSize: fontPointSize
			}
		}

		RowLayout {
			Layout.fillWidth: true
			visible: classComboBox.model.length > 1
			Label {
				text: qsTr("Class:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			CustomComboBox {
				id: classComboBox
				model: {
					let arr1 = [qsTr("No class selected")];
					let arr2 = ClassManager.classNames;
					return arr1.concat(arr2);
				}
				currentIndex: Settings.getValue("grades", "selectedClass") + 1
				onCurrentIndexChanged: updateGrade()
			}
		}

		RowLayout {
			Layout.fillWidth: true
			Label {
				text: qsTr("Grade:")
				font.bold: true
				font.pointSize: fontPointSize
			}
			Label {
				text: validator == null ? "" : gradeCalc.grade
				font.pointSize: fontPointSize
			}
		}
	}
}
