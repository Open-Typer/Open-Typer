/*
 * QmlWindow.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.15
import OpenTyper 1.0

ApplicationWindow {
	property string packName
	property string packFriendlyName
	property string oldPackName: ""
	property bool customPack
	property int currentLesson
	property int currentSublesson
	property int currentAbsoluteSublesson
	property int currentExercise
	property bool customExerciseLoaded
	property int sublessonListStart
	property int lessonCount
	property int sublessonCount
	property int exerciseCount
	property int exerciseLineLength
	property string exerciseText
	property int currentMode
	property bool preview
	property int currentLine
	property int exercisePos
	property int displayPos
	property int absolutePos
	property int linePos
	property int exerciseMistakes
	property int totalHits
	property var recordedCharacters
	property var recordedMistakes
	property int deadKeys
	property bool exerciseInProgress
	property real lastTime
	property bool mistake
	property bool ignoreMistakeAppend
	property int lineCount
	property bool hideText
	property bool blockInput
	Material.theme: ThemeEngine.style === ThemeEngine.DarkStyle ? Material.Dark : Material.Light
	Material.accent: Material.LightBlue // TODO: Use accent color (maybe from ThemeEngine)
	color: ThemeEngine.bgColor
	// TODO: Load window geometry from settings
	width: 1200
	height: 800
	visible: true
	id: root

	ConfigParser {
		id: parser
	}

	ColumnLayout {
		id: mainLayout
		anchors.fill: parent
		spacing: 0
		Panel {
			id: panel1
			Layout.fillWidth: true
			control: RowLayout {
				CustomToolButton {
					objectName: "settingsButton"
					icon.name: "settings"
					toolTipText: qsTr("Options")
				}
				CustomToolButton {
					objectName: "openButton"
					icon.name: "open"
					toolTipText: qsTr("Open")
				}
				CustomToolButton {
					objectName: "printButton"
					icon.name: "print"
					toolTipText: qsTr("Print")
				}
				ToolSeparator {}
				CustomToolButton {
					objectName: "typingTestButton"
					icon.name: "paper"
					text: qsTr("Typing test")
				}
				CustomToolButton {
					objectName: "timedExButton"
					icon.name: "time"
					text: qsTr("Timed exercise")
				}
				CustomToolButton {
					objectName: "errorWordsButton"
					icon.name: "close"
					text: qsTr("Text from error words")
				}
				CustomToolButton {
					objectName: "reverseTextButton"
					icon.name: "rewind"
					text: qsTr("Reverse text")
				}
			}
		}
		Panel {
			id: panel2
			Layout.fillWidth: true
			control: RowLayout {
				property alias closeLoadedExButton: closeLoadedExButton
				property alias statsButton: statsButton
				property alias lessonBox: lessonBox
				property alias sublessonBox: sublessonBox
				property alias exerciseBox: exerciseBox
				CustomToolButton {
					objectName: "repeatExButton"
					icon.name: "repeat"
					toolTipText: qsTr("Repeat exercise")
				}
				CustomToolButton {
					objectName: "closeLoadedExButton"
					id: closeLoadedExButton
					icon.name: "close"
					toolTipText: qsTr("Close loaded exercise")
				}
				CustomToolButton {
					objectName: "previousExButton"
					icon.name: "left"
					toolTipText: qsTr("Previous exercise")
				}
				CustomToolButton {
					objectName: "nextExButton"
					icon.name: "right"
					toolTipText: qsTr("Next exercise")
				}
				CustomComboBox { objectName: "lessonBox"; id: lessonBox }
				CustomComboBox { objectName: "sublessonBox"; id: sublessonBox }
				CustomComboBox { objectName: "exerciseBox"; id: exerciseBox }
				ToolSeparator {}
				CustomToolButton {
					objectName: "statsButton"
					id: statsButton
					icon.name: "stats"
					text: qsTr("Exercise history")
				}
			}
		}
		Paper {
			id: paper
			Layout.fillHeight: true
			Layout.topMargin: 10
			Layout.alignment: Qt.AlignHCenter
		}
	}

	function reload() {
		// Pack name
		packName = Settings.lessonPack();
		console.assert(packName != "");
		var packChanged = false;
		if(packName == oldPackName)
			packChanged = false;
		else
			packChanged = true;
		oldPackName = packName;
		// Custom pack
		customPack = Settings.customLessonPack();
		// Load the pack and start
		if(packChanged)
		{
			loadPack(packName);
			// Reset position
			currentLesson = 1;
			currentSublesson = 1;
			currentExercise = 1;
			repeatExercise();
		}
		else
		{
			updateLessonList();
			panel2.contents.lessonBox.currentIndex = currentLesson - 1;
		}
	}

	function loadPack(name, content = "") {
		customExerciseLoaded = false;
		var packPath = "";
		if(customPack)
			packPath = configName;
		else
			packPath = ":/res/configs/" + packName;
		// Open selected pack
		if(!parser.bufferOpened())
			parser.close();
		if(content === "")
		{
			var bufferOpened = parser.bufferOpened();
			var openSuccess;
			if(bufferOpened && customConfig)
				openSuccess = true;
			else
				openSuccess = parser.open(packPath);
			if(!openSuccess && !bufferOpened)
			{
				Settings.setLessonPack("");
				reload();
				return "";
			}
		}
		else
			parser.loadToBuffer(content);
		// Update lessonBox
		updateLessonList();
		if(customPack)
			packName = packPath;
		// Save selected pack to settings
		Settings.setLessonPack(packName);
		if(customPack)
			packFriendlyName = FileUtils.fileName(packName);
		else
			packFriendlyName = BuiltInPacks.packName(packName);
	}

	function startExercise(lessonID, sublessonID, exerciseID) {
		// Update selected lesson
		panel2.contents.lessonBox.currentIndex = lessonID - 1;
		// Get sublesson count
		sublessonCount = parser.sublessonCount(lessonID);
		// Check if -1 (last sublesson in current lesson) was passed
		if(sublessonID === -1)
			sublessonID = sublessonCount;
		// Update sublesson and exercise lists
		// This must happen before exercise loading!
		loadLesson(lessonID, sublessonID);
		// Check if -1 (last exercise in current sublesson) was passed
		if(exerciseID === -1)
			exerciseID = parser.exerciseCount(lessonID, sublessonID + sublessonListStart);
		// Load line length
		if(!customExerciseLoaded)
			exerciseLineLength = parser.exerciseLineLength(lessonID, sublessonID + sublessonListStart, exerciseID);
		// Load exercise text
		if(customExerciseLoaded)
			exerciseText = customExerciseText;
		else
			exerciseText = parser.exerciseText(lessonID,
				sublessonID + sublessonListStart,
				exerciseID);
		// Get lesson count
		lessonCount = parser.lessonCount();
		// Get exercise count (in current lesson)
		exerciseCount = parser.exerciseCount(lessonID, sublessonID + sublessonListStart);
		// Update level list
		loadSublesson(exerciseID);
		// Make lesson, sublesson and exercise info public
		currentLesson = lessonID;
		currentSublesson = sublessonID;
		currentAbsoluteSublesson = sublessonID + sublessonListStart;
		currentExercise = exerciseID;
		// Init exercise
		initExercise();
	}

	function initExercise() {
		// Init exercise
		PublicPos.currentLesson = currentLesson;
		PublicPos.currentSublesson = currentAbsoluteSublesson;
		PublicPos.currentExercise = currentExercise;
		if(currentMode == 1)
			exerciseText += '\n';
		preview = false;
		currentLine = 0;
		exercisePos = 0;
		displayPos = 0;
		absolutePos = 0;
		linePos = 0;
		exerciseMistakes = 0;
		totalHits = 0;
		recordedCharacters = [];
		recordedMistakes = [];
		deadKeys = 0;
		exerciseInProgress = false;
		lastTime = 0;
		mistake = false;
		ignoreMistakeAppend = false;
		paper.mistake = "";
		panel2.contents.closeLoadedExButton.visible = customExerciseLoaded;
		// Init input
		paper.input = "";
		updateText();
		// Enable/disable stats
		var enableStats = !customExerciseLoaded && !customPack && (currentMode == 0);
		panel2.contents.statsButton.enabled = enableStats;
	}

	function updateText() {
		paper.currentLineVisible = true;
		paper.remainingVisible = true;
		// TODO: Hide export button here
		var displayExercise = parser.initExercise(exerciseText, exerciseLineLength);
		lineCount = StringUtils.charCount(displayExercise, '\n');
		// Process exercise text
		var text = exerciseText;
		if(currentMode == 1)
			text = exerciseText.substring(0, exerciseText.length - 1);
		var finalText = parser.initExercise(text, exerciseLineLength, false, currentLine);
		if(currentMode == 1)
			finalText += "\n" + displayExercise.substring(0, displayLevel.count() - 1).repeat(100 / lineCount);
		var currentLineText = "";
		var remainingText = "";
		var line = 0;
		for(var i = 0; i < finalText.length; i++)
		{
			remainingText += finalText[i];
			if((finalText[i] === '\n') || (i + 1 >= finalText.length))
			{
				if(line == 0)
				{
					currentLineText = remainingText;
					remainingText = "";
					if(currentLineText[currentLineText.length - 1] === '\n')
						currentLineText.replace(currentLineText.length - 1, '');
				}
				line++;
			}
		}
		paper.currentLine = currentLineText;
		paper.remaining = remainingText;
		if(hideText)
		{
			paper.currentLineVisible = false;
			paper.remaining = "";
		}
		blockInput = false;
	}

	function repeatExercise() {
		startExercise(currentLesson, currentSublesson, currentExercise);
	}

	function updateLessonList() {
		var lessons = [];
		var lessonDesc;
		var count = parser.lessonCount();
		for(var i = 1; i <= count; i++)
		{
			lessonDesc = parser.parseDesc(parser.lessonDesc(i));
			if(lessonDesc === "")
				lessons[i - 1] = parser.lessonTr(i);
			else
				lessons[i - 1] = parser.lessonTr(i) + " " + lessonDesc;
		}
		panel2.contents.lessonBox.model = lessons;
	}

	function loadLesson(lessonID, sublessonID) {
		// Sublessons
		var sublessons = [];
		sublessonListStart = 0;
		var sublesson_i = 0, i2 = 0;
		for(var i = 1; i <= sublessonCount + i2; i++)
		{
			if(parser.exerciseCount(lessonID, i) > 0)
			{
				sublessons[sublesson_i] = parser.sublessonName(i);
				sublesson_i++;
			}
			else
			{
				i2++;
				if(sublessonID + i2 > i)
					sublessonListStart++;
			}
		}
		panel2.contents.sublessonBox.model = sublessons;
		panel2.contents.sublessonBox.currentIndex = sublessonID - 1;
	}

	function loadSublesson(exerciseID) {
		// Exercises
		var exercises = [];
		for(var i = 1; i <= exerciseCount; i++)
			exercises[i - 1] = parser.exerciseTr(i);
		panel2.contents.exerciseBox.model = exercises;
		panel2.contents.exerciseBox.currentIndex = exerciseID - 1;
	}

	Component.onCompleted: reload();
}
