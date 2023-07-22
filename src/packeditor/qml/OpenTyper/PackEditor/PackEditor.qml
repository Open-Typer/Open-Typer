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
	readonly property bool saved: editorModel.saved
	signal aboutToClose()
	id: root
	spacing: 0

	QtObject {
		property bool skipChange: false
		property bool closeOnSave: false
		id: priv
	}

	function newFile() {
		editorModel.newFile();
	}

	function close() {
		if(!saved)
			saveDialog.open();
		else
			forceClose();
	}

	function forceClose() {
		aboutToClose();
		destroy();
	}

	PackEditorModel {
		id: editorModel
		fileName: root.fileName
		onFileNameChanged: root.fileName = fileName
		onCurrentRawTextChanged: {
			priv.skipChange = true;
			inputEdit.text = currentRawText;
		}
		onCurrentLengthLimitChanged: {
			priv.skipChange = true;
			lengthLimitBox.value = currentLengthLimit;
		}
		onCurrentLineLengthChanged: {
			priv.skipChange = true;
			lineLengthBox.value = currentLineLength;
		}
	}

	MessageBox {
		id: saveDialog
		text: qsTr("Do you want to save your changes?")
		standardButtons: Dialog.Save | Dialog.Discard | Dialog.Cancel
		icon: MessageBox.Question
		onAccepted: {
			priv.closeOnSave = true;
			save();
		}
		onDiscarded: forceClose()
	}

	QmlFileDialog {
		id: fileDialog
		nameFilters: [qsTr("Open-Typer pack files") + " (*.typer)"]
		defaultSuffix: "typer"
		// TODO: Set defaultSuffix based on selected filter in QmlFileDialog
		showAllFiles: false
	}

	function saveAs() {
		let fileName = fileDialog.getSaveFileName();

		if(fileName !== "")
		{
			editorModel.saveAs(fileName);
			if(priv.closeOnSave)
				forceClose();
		}

		priv.closeOnSave = false;
	}

	function save() {
		if(editorModel.readOnly)
			saveAs();
		else
			editorModel.save();
	}

	Panel {
		id: panel1
		Layout.fillWidth: true
		control: RowLayout {
			CustomToolButton {
				enabled: !editorModel.saved
				icon.name: "save"
				toolTipText: qsTr("Save")
				onClicked: save()
			}

			CustomToolButton {
				icon.name: "save-as"
				toolTipText: qsTr("Save as...")
				onClicked: saveAs()
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
						onTriggered: editorModel.addLesson()
					}

					CustomMenu {
						id: addSublessonMenu
						title: qsTr("Sublesson")
						enabled: editorModel.unusedSublessons.length > 0

						// TODO: Use a Repeater after fully switching to Qt 6
						/*Repeater {
							id: addSublessonRepeater
							model: editorModel.unusedSublessons

							CustomMenuItem {
								text: editorModel.sublessonName(modelData)
								onTriggered: editorModel.addSublesson(modelData)
							}
						}*/

						CustomMenuItem {
							text: editorModel.sublessonName(1)
							enabled: editorModel.unusedSublessons.includes(1)
							onTriggered: editorModel.addSublesson(1)
						}

						CustomMenuItem {
							text: editorModel.sublessonName(2)
							enabled: editorModel.unusedSublessons.includes(2)
							onTriggered: editorModel.addSublesson(2)
						}

						CustomMenuItem {
							text: editorModel.sublessonName(3)
							enabled: editorModel.unusedSublessons.includes(3)
							onTriggered: editorModel.addSublesson(3)
						}

						CustomMenuItem {
							text: editorModel.sublessonName(4)
							enabled: editorModel.unusedSublessons.includes(4)
							onTriggered: editorModel.addSublesson(4)
						}
					}

					CustomMenuItem {
						text: qsTr("Exercise")
						onTriggered: editorModel.addExercise()
					}
				}
			}

			CustomToolButton {
				enabled: editorModel.canRemove
				icon.name: "left"
				toolTipText: qsTr("Previous exercise")
				onClicked: editorModel.previousExercise()
			}

			CustomToolButton {
				enabled: editorModel.canRemove
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

			CustomToolButton {
				enabled: editorModel.canRemove
				icon.name: "delete"
				toolTipText: qsTr("Remove exercise")
				onClicked: editorModel.removeCurrentExercise()
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
					onValueChanged: {
						if(!priv.skipChange)
							editorModel.currentLengthLimit = value
						else
							priv.skipChange = false
					}
				}
			}

			RowLayout {
				Label {
					text: qsTr("Line length:")
				}

				CustomSpinBox {
					id: lineLengthBox
					from: 30
					to: 120
					stepSize: 10
					onValueChanged: {
						if(!priv.skipChange)
							editorModel.currentLineLength = value
						else
							priv.skipChange = false;
					}
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
				onTextChanged: {
					if(!priv.skipChange)
						editorModel.currentRawText = text
					else
						priv.skipChange = false;
				}

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
