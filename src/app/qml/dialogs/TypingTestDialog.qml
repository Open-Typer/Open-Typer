/*
 * TypingTestDialog.qml
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
import OpenTyper.UiComponents 1.0

CustomDialog {
	property var parserObj: null
	property string exerciseText
	property bool timed: contents.timeLimitCheckBox.checked
	property int timeLimitSecs: contents.timeSecs
	property bool correctMistakes: contents.correctMistakesCheckBox.checked
	property bool lockUi: contents.lockUiCheckBox.checked
	property bool hideText: contents.hideTextCheckBox.checked
	property bool firstLoad
	id: control
	title: qsTr("Typing test")
	standardButtons: Dialog.Cancel | Dialog.Ok
	onAboutToShow: {
		console.assert(parserObj != null);
		firstLoad = true;
		contents.lessonBox.load();
		contents.sublessonBox.load();
		contents.exerciseBox.load();
		firstLoad = false;
	}
	function updateText() {
		var lesson = contents.lessonBox.currentIndex + 1;
		var sublesson = contents.sublessonBox.currentSublesson();
		var exercise = contents.exerciseBox.currentIndex + 1;
		exerciseText = parserObj.initExercise(parserObj.exerciseText(lesson, sublesson, exercise),
							  parserObj.exerciseLineLength(lesson, sublesson, exercise));
	}
	contentItem: ColumnLayout {
		property alias lessonBox: lessonBox
		property alias sublessonBox: sublessonBox
		property alias exerciseBox: exerciseBox
		property alias timeLimitCheckBox: timeLimitCheckBox
		property alias correctMistakesCheckBox: correctMistakesCheckBox
		property alias lockUiCheckBox: lockUiCheckBox
		property alias hideTextCheckBox: hideTextCheckBox
		property int timeSecs: timeLimitLoader.item.timeSecs
		property string fileContent
		property bool fileLoaded: false
		Binding {
			target: control.standardButton(Dialog.Ok)
			property: "enabled"
			value: fromFileButton.checked && fileLoaded || !fromFileButton.checked
		}
		Label {
			//: Typing test text source
			text: qsTr("Source")
			font.bold: true
			font.pointSize: 12
		}
		RadioButton {
			id: selectedExerciseButton
			text: qsTr("Selected exercise")
			checked: true
			onCheckedChanged: {
				if(checked)
					updateText();
			}
		}
		RowLayout {
			enabled: selectedExerciseButton.checked
			CustomComboBox {
				id: lessonBox
				onCurrentIndexChanged: {
					sublessonBox.load();
					exerciseBox.load();
				}
				function load() {
					var tmpList = [];
					var count = parserObj.lessonCount();
					for(var i = 1; i <= count; i++)
						tmpList.push(parserObj.lessonTr(i) + " " + parserObj.parseDesc(parserObj.lessonDesc(i)));
					model = tmpList;
					if(firstLoad)
						currentIndex = parserObj.currentLesson() - 1;
				}
			}
			CustomComboBox {
				property int delta: 0
				id: sublessonBox
				onCurrentIndexChanged: { exerciseBox.load(); }
				function load() {
					var tmpList = [];
					var count = parserObj.sublessonCount(lessonBox.currentIndex + 1);
					var localIndex = 0, newIndex = -1;
					delta = 0;
					for(var i = 1; i <= count + delta; i++)
					{
						if(parserObj.exerciseCount(lessonBox.currentIndex + 1, i) === 0)
							delta++;
						else
						{
							if((parserObj.currentSublesson() === i) && (newIndex == -1))
								newIndex = localIndex;
							localIndex++;
							tmpList.push(parserObj.sublessonName(i));
						}
					}
					model = tmpList;
					if(firstLoad)
						currentIndex = newIndex;
					else
						currentIndex = 0;
				}
				function currentSublesson() {
					var count = parserObj.sublessonCount(lessonBox.currentIndex + 1);
					var localDelta = 0, localIndex = 0;
					var out = 1;
					for(var i = 1; i <= count + localDelta; i++)
					{
						if(parserObj.exerciseCount(lessonBox.currentIndex + 1, i) === 0)
							localDelta++;
						else
						{
							if(localIndex == currentIndex)
								break;
							localIndex++;
						}
						out++;
					}
					return out;
				}
			}
			CustomComboBox {
				id: exerciseBox
				onCurrentIndexChanged: updateText()
				function load() {
					var tmpList = [];
					var count = parserObj.exerciseCount(lessonBox.currentIndex + 1, sublessonBox.currentSublesson());
					for(var i = 1; i <= count; i++)
						tmpList.push(parserObj.exerciseTr(i));
					model = tmpList;
					if(firstLoad)
						currentIndex = parserObj.currentExercise() - 1;
					else
						currentIndex = 0;
					updateText()
				}
			}
			CustomButton {
				text: qsTr("Preview")
				font.capitalization: Font.MixedCase
				onClicked: {
					var lesson = lessonBox.currentIndex + 1;
					var sublesson = sublessonBox.currentSublesson();
					var exercise = exerciseBox.currentIndex + 1;
					var exerciseText = parserObj.initExercise(parserObj.exerciseText(lesson, sublesson, exercise),
										  parserObj.exerciseLineLength(lesson, sublesson, exercise));
					previewToolTip.show(exerciseText, 5000);
				}
				ToolTip {
					id: previewToolTip
					font.pointSize: 12
					Binding on width {
						value: {
							var maxWidth = 0;
							var lines = previewToolTip.text.split("\n");
							for(var i = 0; i < lines.length; i++)
								maxWidth = Math.max(maxWidth, previewToolTipMetrics.boundingRect(lines[i]).width);
							return maxWidth + 50;
						}
					}
					x: -width / 2
					clip: true
					FontMetrics {
						id: previewToolTipMetrics
						font: previewToolTip.font
					}
				}
			}
		}
		RadioButton {
			id: fromFileButton
			text: qsTr("From file")
		}
		RowLayout {
			enabled: fromFileButton.checked
			CustomButton {
				id: browseButton
				text: qsTr("Browse")
				font.capitalization: Font.MixedCase
				onClicked: fileDialog.getOpenFileContent();
				function showWarning() {
					ToolTip.show(qsTr("This file is too large!"), 5000);
				}
			}
			Label {
				id: fileNameLabel
				text: qsTr("(no file selected)")
			}
		}
		MenuSeparator { Layout.fillWidth: true }
		Label {
			//: Typing test options
			text: qsTr("Options")
			font.bold: true
			font.pointSize: 12
		}
		CheckBox {
			id: timeLimitCheckBox
			text: qsTr("Time limited")
		}
		Loader {
			id: timeLimitLoader
			visible: timeLimitCheckBox.checked
			sourceComponent: timeDialog.contentItem
		}
		MenuSeparator { Layout.fillWidth: true }
		CheckBox {
			id: correctMistakesCheckBox
			text: qsTr("Character correction after mistake (by program)")
			checked: true
			onCheckedChanged: {
				if(checked)
					hideTextCheckBox.checked = false;
			}
		}
		CheckBox {
			id: lockUiCheckBox
			text: qsTr("Lock user interface")
		}
		CheckBox {
			id: hideTextCheckBox
			enabled: !correctMistakesCheckBox.checked
			text: qsTr("Hide text")
		}
		QmlFileDialog {
			id: fileDialog
			onFileContentReady: {
				if(content.length > 8192) // Maximum size
					browseButton.showWarning();
				else
				{
					fileContent = parserObj.initExercise(content, parserObj.defaultLineLength());
					exerciseText = fileContent;
					fileNameLabel.text = shortFileName;
					fileLoaded = true;
				}
			}
		}
	}
	TimeDialog { id: timeDialog }
}
