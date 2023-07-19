/*
 * PackEditor.qml
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
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0
import OpenTyper.LessonPack 1.0
import OpenTyper.PackEditor 1.0

ColumnLayout {
	property string fileName
	property int currentLesson
	property int currentSublesson
	property int currentExercise
	property int sublessonListStart
	property int lessonCount
	property int sublessonCount
	property int exerciseCount
	id: root
	spacing: 0

	PackEditorModel {
		id: editorModel
		fileName: root.fileName
		onCurrentRawTextChanged: inputEdit.text = currentRawText
		onCurrentLengthLimitChanged: lengthLimitBox.value = currentLengthLimit
	}

	Panel {
		id: panel1
		Layout.fillWidth: true
		control: RowLayout {
			CustomToolButton {
				icon.name: "save"
				toolTipText: qsTr("Save")
			}

			CustomToolButton {
				icon.name: "save-as"
				toolTipText: qsTr("Save as...")
			}

			CustomToolButton {
				icon.name: "undo"
				toolTipText: qsTr("Undo")
				enabled: inputEdit.canUndo
				onClicked: inputEdit.undo()
			}

			CustomToolButton {
				icon.name: "redo"
				toolTipText: qsTr("Redo")
				enabled: inputEdit.canRedo
				onClicked: inputEdit.redo()
			}
		}
	}

	Panel {
		id: panel2
		Layout.fillWidth: true
		control: RowLayout {
			property alias lessonBox: lessonBox
			property alias sublessonBox: sublessonBox
			property alias exerciseBox: exerciseBox

			CustomToolButton {
				icon.name: "add"
				toolTipText: qsTr("Add...")
				onClicked: addMenu.toggle()

				CustomMenu {
					id: addMenu

					CustomMenuItem {
						text: qsTr("Lesson")
					}

					CustomMenuItem {
						text: qsTr("Sublesson")
					}

					CustomMenuItem {
						text: qsTr("Exercise")
					}
				}
			}

			CustomToolButton {
				icon.name: "left"
				toolTipText: qsTr("Previous exercise")
				onClicked: editorModel.previousExercise()
			}

			CustomToolButton {
				icon.name: "right"
				toolTipText: qsTr("Next exercise")
				onClicked: editorModel.nextExercise()
			}

			CustomComboBox {
				id: lessonBox
				model: editorModel.lessonList
				currentIndex: editorModel.lesson - 1
				onActivated: editorModel.lesson = currentIndex + 1
			}

			CustomComboBox {
				id: sublessonBox
				model: editorModel.sublessonList
				currentIndex: editorModel.sublesson - 1
				onActivated: editorModel.sublesson = currentIndex + 1
			}

			CustomComboBox {
				id: exerciseBox
				model: editorModel.exerciseList
				currentIndex: editorModel.exercise - 1
				onActivated: editorModel.exercise = currentIndex + 1
			}
		}
	}

	Rectangle {
		// TODO: Move this into a popup
		radius: 10
		border.color: ThemeEngine.borderColor
		color: ThemeEngine.paperColor
		Layout.topMargin: 10
		Layout.fillWidth: true
		height: exerciseOptionsLayout.height + exerciseOptionsLayout.anchors.margins * 2

		ColumnLayout {
			id: exerciseOptionsLayout
			anchors.left: parent.left
			anchors.top: parent.top
			anchors.margins: 10

			RowLayout {
				Label {
					text: qsTr("Repeating:")
				}

				CustomComboBox {
					id: repeatingComboBox
					model: [qsTr("None"), qsTr("Words")]
					currentIndex: editorModel.currentRepeatType == "w" ? 1 : 0
					onActivated: editorModel.currentRepeatType = currentIndex == 1 ? "w" : "0"
				}
			}

			RowLayout {
				enabled: repeatingComboBox.currentIndex > 0

				Label {
					text: qsTr("Length limit:")
				}

				CustomSpinBox {
					id: lengthLimitBox
					from: 1
					to: 729
					stepSize: 10
					onValueChanged: editorModel.currentLengthLimit = value
				}
			}

			RowLayout {
				Label {
					text: qsTr("Line length:")
				}

				CustomSpinBox {
					from: 30
					to: 120
					stepSize: 10
				}
			}
		}
	}

	Rectangle {
		radius: 10
		border.color: ThemeEngine.borderColor
		color: ThemeEngine.paperColor
		Layout.topMargin: 10
		Layout.fillWidth: true
		height: 250

		Label {
			id: inputLabel
			text: qsTr("Input text:")
			font.pointSize: 14
			font.bold: true
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.margins: 10
		}

		CustomFlickable {
			id: inputFlickable
			anchors.top: inputLabel.bottom
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: 10
			contentWidth: Math.max(width, inputMetrics.boundingRect(inputEdit.text).width)
			contentHeight: Math.max(1, inputEdit.text.split('\n').length) * inputMetrics.height
			showHorizontalScrollBar: true
			showVerticalScrollBar: true
			clip: true

			TextEdit {
				id: inputEdit
				font: ThemeEngine.font
				color: ThemeEngine.foregroundColor
				width: inputFlickable.contentWidth
				height: Math.max(inputFlickable.height, inputFlickable.contentHeight)
				onTextChanged: editorModel.currentRawText = text

				FontMetrics {
					id: inputMetrics
					font: inputEdit.font
				}
			}
		}
	}

	Rectangle {
		radius: 10
		border.color: ThemeEngine.borderColor
		color: ThemeEngine.paperColor
		Layout.topMargin: 10
		Layout.fillWidth: true
		Layout.fillHeight: true
		implicitHeight: 100

		Label {
			id: resultLabel
			text: qsTr("Result:")
			font.pointSize: 14
			font.bold: true
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.margins: 10
		}

		CustomFlickable {
			id: resultFlickable
			anchors.top: resultLabel.bottom
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: 10
			contentWidth: Math.max(width, calculateTextWidth(resultEdit.text))
			contentHeight: Math.max(1, resultEdit.text.split('\n').length) * resultMetrics.height
			showHorizontalScrollBar: true
			showVerticalScrollBar: true
			clip: true

			function calculateTextWidth(text) {
				let lines = text.split("\n");
				let maxValue = 0;
				for(var i = 0; i < lines.length; i++)
					maxValue = Math.max(maxValue, resultMetrics.boundingRect(lines[i]).width);
				return maxValue;
			}

			TextEdit {
				id: resultEdit
				font: ThemeEngine.font
				color: ThemeEngine.exerciseTextColor
				width: resultFlickable.contentWidth
				height: Math.max(resultFlickable.height, resultFlickable.contentHeight)
				readOnly: true
				text: editorModel.currentText

				FontMetrics {
					id: resultMetrics
					font: resultEdit.font
				}
			}
		}
	}
}
