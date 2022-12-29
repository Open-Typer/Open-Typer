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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
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
	property string customExerciseText
	property int sublessonListStart
	property int lessonCount
	property int sublessonCount
	property int exerciseCount
	property int exerciseLineLength
	property string exerciseText
	property string displayExercise
	property string fullInput
	property int currentMode: 0
	property bool preview
	property int currentLine
	property int exercisePos
	property int displayPos
	property int absolutePos
	property int linePos
	property int exerciseMistakes
	property int totalHits
	property int netHits
	property var recordedCharacters
	property var recordedMistakes
	property int deadKeys
	property bool exerciseInProgress
	property real lastTime
	property bool mistake
	property bool ignoreMistakeAppend
	property int lineCount
	property bool hideText: false
	property bool blockInput
	property bool timedExStarted
	property int timedExTime
	property int timedExCountdown
	property var errorWords: []
	property bool correctMistakes: true
	property bool eventInProgress: false
	property bool testLoaded: false
	property bool uiLocked: false
	property string formattedExerciseTime
	property string formattedExerciseRemainingTime
	Material.theme: ThemeEngine.style === ThemeEngine.DarkStyle ? Material.Dark : Material.Light
	Material.accent: Material.LightBlue // TODO: Use accent color (maybe from ThemeEngine)
	color: ThemeEngine.bgColor
	minimumWidth: mainLayout.minWidth
	minimumHeight: mainLayout.minHeight
	// TODO: Load window geometry from settings
	width: 1200
	height: 800
	visible: true
	id: root

	ConfigParser {
		id: parser
	}

	ExerciseTimer {
		id: exerciseTimer
	}

	Timer {
		id: updateTimer
		interval: 16
		repeat: true
		running: true
		onTriggered: {
			exerciseTimer.update();
			eventInProgress = false;
			formattedExerciseTime = exerciseTimer.formattedTime();
			if((currentMode == 1) && timedExStarted)
			{
				formattedExerciseRemainingTime = exerciseTimer.formattedRemainingTime(timedExTime * 1000);
				if((exerciseTimer.elapsed / 1000 >= timedExTime) && exerciseInProgress)
				{
					lastTime = timedExTime;
					changeMode(0);
					endExercise();
				}
			}
			else if(currentMode == 1)
			{
				formattedExerciseRemainingTime = exerciseTimer.formattedRemainingTime((timedExCountdown + 1) * 1000);
				paper.input = timedExCountdown - Math.floor(exerciseTimer.elapsed / 1000) + "...";
				if(exerciseTimer.elapsed / 1000 > timedExCountdown)
				{
					timedExStarted = true;
					exerciseTimer.start();
					blockInput = false;
					restart();
					paper.input = "";
				}
			}
		}
	}

	QmlFileDialog {
		id: customExFileDialog
		nameFilters: [qsTr("Text files") + "(*.txt)"]
		onFileContentReady: {
			if(content.length > 8192) // Maximum size
				largeFileBox.open();
			else
			{
				exerciseLineLength = parser.defaultLineLength();
				loadText(content, true);
			}
		}
	}

	Binding {
		target: statsDialog
		property: "packName"
		value: packName
	}

	Binding {
		target: statsDialog
		property: "lesson"
		value: currentLesson
	}

	Binding {
		target: statsDialog
		property: "sublesson"
		value: currentAbsoluteSublesson
	}

	Binding {
		target: statsDialog
		property: "exercise"
		value: currentExercise
	}

	ColumnLayout {
		property int minWidth: Math.max(implicitWidth, paper.paperRect.width)
		property int minHeight: implicitHeight
		id: mainLayout
		anchors.fill: parent
		spacing: 0

		CustomMenuBar {
			id: customMenuBar
			Layout.fillWidth: true
			enabled: !uiLocked
		}

		Rectangle {
			visible: customMenuBar.visible
			Layout.fillWidth: true
			implicitHeight: 1
			color: Material.theme == Material.Dark ? Qt.rgba(1, 1, 1, 0.2) : Qt.rgba(0, 0, 0, 0.2);
		}

		Panel {
			id: panel1
			Layout.fillWidth: true
			visible: currentMode == 0
			enabled: !uiLocked
			control: RowLayout {
				CustomToolButton {
					iconName: "settings"
					toolTipText: qsTr("Options")
					onClicked: {
						optionsWindow.exec();
					}
				}
				CustomToolButton {
					iconName: "open"
					toolTipText: qsTr("Open")
					onClicked: customExFileDialog.getOpenFileContent()
				}
				CustomToolButton {
					iconName: "print"
					toolTipText: qsTr("Print")
					visible: !QmlUtils.osWasm();
					onClicked: {
						if(customExerciseLoaded)
							QmlUtils.printExercise(displayExercise);
						else
							QmlUtils.printExercise(currentLesson, currentAbsoluteSublesson, currentExercise, displayExercise);
					}
				}
				ToolSeparator {}
				CustomToolButton {
					iconName: "paper"
					text: qsTr("Typing test")
					onClicked: {
						// TODO: Send events
						/*AddonApi::clearLoadExTargets();
						AddonApi::sendEvent(IAddon::Event_OpenLoadExDialog);*/
						// TODO: Add targets support
						/*QMap<int, QString> targets = AddonApi::loadExTargets();
						if(targets.count() == 0)
							dialog = new LoadExerciseDialog(this);
						else
							dialog = new LoadExerciseDialog(targets, this);*/
						loadExerciseDialog.init();
						loadExerciseDialog.exec();
					}
				}
				CustomToolButton {
					iconName: "time"
					text: qsTr("Timed exercise")
					onClicked: timeDialog.open()
				}
				CustomToolButton {
					iconName: "close"
					text: qsTr("Text from error words")
					onClicked: {
						if(errorWords.length == 0)
						{
							noErrorWordsBox.open();
							return;
						}
						var wordCount = 25;
						if(errorWords.length > wordCount)
							wordCount = errorWords.length;
						var usedWords = [], outputWords = [];
						var index = 0;
						for(var i = 0; i < wordCount; i++)
						{
							var word;
							if(i % 5 == 0)
							{
								do
								{
									index = Math.floor(Math.random() * errorWords.length);
									word = errorWords[index];
								} while(usedWords.indexOf(word) != -1);
								word = errorWords[index];
								usedWords[usedWords.length] = word;
							}
							outputWords[outputWords.length] = word;
							if(usedWords.length == errorWords.length)
								usedWords = [];
						}
						loadText(outputWords.join(" "), false);
					}
				}
				CustomToolButton {
					iconName: "rewind"
					text: qsTr("Reverse text")
					onClicked: {
						var oldText = parser.initText(exerciseText);
						var newText = "";
						for(var i = oldText.length - 1; i >= 0; i--)
							newText += exerciseText[i];
						loadText(newText, true);
					}
				}
			}
		}
		Panel {
			id: panel2
			Layout.fillWidth: true
			visible: currentMode == 0
			enabled: !uiLocked
			control: RowLayout {
				property alias closeLoadedExButton: closeLoadedExButton
				property alias statsButton: statsButton
				property alias lessonBox: lessonBox
				property alias sublessonBox: sublessonBox
				property alias exerciseBox: exerciseBox
				CustomToolButton {
					iconName: "repeat"
					toolTipText: qsTr("Repeat exercise")
					onClicked: repeatExercise();
				}
				CustomToolButton {
					id: closeLoadedExButton
					iconName: "close"
					toolTipText: qsTr("Close loaded exercise")
					onClicked: {
						customExerciseLoaded = false
						repeatExercise();
					}
				}
				CustomToolButton {
					iconName: "left"
					toolTipText: qsTr("Previous exercise")
					onClicked: previousExercise();
				}
				CustomToolButton {
					iconName: "right"
					toolTipText: qsTr("Next exercise")
					onClicked: nextExercise();
				}
				CustomComboBox { id: lessonBox; onActivated: selectLesson(index); }
				CustomComboBox { id: sublessonBox; onActivated: selectSublesson(index); }
				CustomComboBox { id: exerciseBox; onActivated: selectExercise(index); }
				ToolSeparator {}
				CustomToolButton {
					id: statsButton
					iconName: "stats"
					text: qsTr("Exercise history")
					onClicked: {
						// TODO: Send load stats event
						statsDialog.loadData();
						statsDialog.exec();
					}
				}
			}
		}
		Panel {
			id: timedExPanel
			Layout.fillWidth: true
			fillWidth: true
			visible: currentMode == 1
			enabled: !uiLocked
			control: RowLayout {
				Item { Layout.fillWidth: true }
				Label {
					text: formattedExerciseRemainingTime
					color: Material.theme === Material.Light ? "black" : "white"
					font.pointSize: 20
					font.bold: true
				}
				CustomToolButton {
					visible: !uiLocked
					iconName: "close"
					toolTipText: qsTr("Stop")
					onClicked: {
						changeMode(0);
						repeatExercise();
					}
				}
				Item { Layout.fillWidth: true }
			}
		}
		Paper {
			id: paper
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.topMargin: 10
			onKeyPressed: keyPress(event);
			onKeyReleased: console.log("released: " + event["text"]);
		}
	}

	MessageBox {
		id: largeFileBox
		windowTitle: qsTr("Error");
		title: qsTr("This file is too large!")
		icon: critical
	}

	TimeDialog {
		id: timeDialog
		onAccepted: startTimedExercise(timeSecs)
	}

	MessageBox {
		id: noErrorWordsBox
		windowTitle: qsTr("No error words");
		title: qsTr("You don't have any error words.")
		icon: information
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

	function loadPack(name) {
		return loadPackContent(name, "");
	}

	function loadPackContent(name, content) {
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
		// Update exercise list
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
		parser.setCurrentLesson(currentLesson);
		parser.setCurrentSublesson(currentAbsoluteSublesson);
		parser.setCurrentExercise(currentExercise);
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
		fullInput = "";
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
		displayExercise = parser.initExercise(exerciseText, exerciseLineLength);
		lineCount = StringUtils.charCount(displayExercise, '\n');
		// Process exercise text
		var text = exerciseText;
		if(currentMode == 1)
			text = exerciseText.substring(0, exerciseText.length - 1);
		var finalText = parser.initExercise(text, exerciseLineLength, false, currentLine);
		if(currentMode == 1)
			finalText += "\n" + StringUtils.repeatString(displayExercise.substring(0, displayExercise.length - 1), 100 / lineCount);
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
						currentLineText = currentLineText.substring(0, currentLineText.length - 1);
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

	function nextExercise() {
		customExerciseLoaded = false;
		if(currentExercise == exerciseCount)
		{
			if(currentSublesson == sublessonCount)
			{
				if(currentLesson == lessonCount)
					currentLesson = 1;
				else
					currentLesson++;
				currentSublesson = 1;
			}
			else
				currentSublesson++;
			currentExercise = 1;
		}
		else
			currentExercise++;
		repeatExercise();
	}

	function previousExercise() {
		customExerciseLoaded = false;
		if(currentExercise == 1)
		{
			if(currentSublesson == 1)
			{
				if(currentLesson == 1)
					currentLesson = lessonCount;
				else
					currentLesson--;
				currentSublesson = -1;
			}
			else
				currentSublesson--;
			currentExercise = -1;
		}
		else
			currentExercise--;
		repeatExercise();
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

	function selectLesson(index) {
		currentLesson = index + 1;
		currentSublesson = 1;
		currentExercise = 1;
		customExerciseLoaded = false;
		repeatExercise();
	}

	function selectSublesson(index) {
		currentSublesson = index + 1;
		currentExercise = 1;
		customExerciseLoaded = false;
		repeatExercise();
	}

	function selectExercise(index) {
		currentExercise = index + 1;
		customExerciseLoaded = false;
		repeatExercise();
	}

	function keyPress(event) {
		if(eventInProgress || blockInput || ((currentMode == 1) && !timedExStarted))
			return;
		var keyID = event["key"];
		var highlightID = keyID;
		if((fullInput.length < exerciseText.length) && (keyID === Qt.Key_Shift))
		{
			// TODO: Get shift key based on next character and set a special highlight ID
		}
		// TODO: Highlight the key on the keyboard
		if(event["isAutoRepeat"])
			return;
		if(KeyboardUtils.isDeadKey(keyID))
		{
			deadKeys++;
			// Count modifier key used with the dead key
			if(event["modifiers"] !== Qt.NoModifier)
				deadKeys++;
			return;
		}
		if(KeyboardUtils.isSpecialKey(event) && (keyID !== Qt.Key_Backspace))
			return;
		eventInProgress = true;
		if((exercisePos == 0) && !exerciseInProgress)
		{
			errorWords = [];
			exerciseTimer.start();
			exerciseInProgress = true;
		}
		var ignoreBackspace = false;
		var keyText = event["text"];
		if((keyText === "'") && (displayExercise[displayPos] === "‘"))
			keyText = "‘";
		if((keyText === "‘") && (displayExercise[displayPos] === '\''))
			keyText = "'";
		if((keyID === Qt.Key_Return) || (keyID === Qt.Key_Enter))
			keyText = "\n";
		var correctChar = ((((displayExercise[displayPos] === '\n') && ((keyID === Qt.Key_Return) || (keyID === Qt.Key_Enter) || (keyID === Qt.Key_Space))) || ((displayExercise[displayPos] !== '\n') && (keyText === exerciseText[exercisePos]))) && !mistake);
		if(correctChar || !correctMistakes)
		{
			if(!mistake && ignoreMistakeAppend)
				paper.mistake += "_";
			if(keyID === Qt.Key_Backspace)
			{
				fullInput = fullInput.substring(0, fullInput.length - 1);
				var text = paper.input;
				if(text.length == 0)
				{
					if(currentLine == 0)
						paper.input = text.substring(0, text.length - 1);
					else if(fullInput.length > 0)
					{
						var arr = fullInput.split('\n');
						paper.input = arr[arr.length - 1];
						currentLine--;
						linePos = 0;
						updateText();
					}
				}
				else
					paper.input = text.substring(0, text.length - 1);
				if(fullInput.length > 0)
				{
					exercisePos--;
					displayPos--;
					absolutePos--;
					linePos--;
					recordedCharacters.pop();
				}
			}
			else
			{
				if(ignoreMistakeAppend)
					paper.mistake = paper.mistake.substring(0, paper.mistake.length - 1);
				if((((keyText === "\n") || ((keyText === " ") && correctMistakes)) && (displayExercise[displayPos] === '\n')) || (keyText === "\n"))
				{
					if(hideText)
					{
						paper.input += "\n";
						paper.mistake += "\n";
					}
					else
					{
						paper.input = "";
						paper.mistake = "";
					}
					linePos = 0;
					keyText = "\n";
					currentLine++;
					ignoreMistakeAppend = false;
					updateText();
					if((currentMode == 1) && (currentLine >= lineCount - 1))
					{
						currentLine = 0;
						updateText();
						exercisePos = -1;
						displayPos = -1;
						linePos = -1;
						deadKeys = 0;
						mistake = false;
						paper.input = "";
					}
				}
				else
				{
					if(ignoreMistakeAppend)
						ignoreMistakeAppend = false;
					else if(correctMistakes)
						paper.mistake += keyText === "\n" ? "\n" : " ";
					paper.input += keyText;
					linePos++;
				}
				fullInput += keyText;
				exercisePos++;
				displayPos++;
				absolutePos++;
				var charHits = 1;
				// Count modifier keys
				if(event["modifiers"] !== Qt.NoModifier)
					charHits++;
				// Count dead keys
				charHits += deadKeys;
				totalHits += charHits;
				var charRecord = Qt.createQmlObject("import OpenTyper 1.0; CharacterRecord {}", root);
				charRecord.keyText = keyText;
				charRecord.hits = charHits;
				recordedCharacters[recordedCharacters.length] = charRecord;
				deadKeys = 0;
			}
		}
		else
		{
			if(mistake)
			{
				deadKeys = 0;
				if(keyID === Qt.Key_Backspace)
				{
					mistake = false;
					ignoreMistakeAppend = true;
					paper.mistake = paper.mistake.substring(0, paper.mistake.length - 1);
					paper.input = paper.input.substring(0, paper.input.length - 1);
				}
			}
			else
			{
				if(!KeyboardUtils.isSpecialKey(event))
				{
					var newRecordedMistakes = [];
					var removeCount = 0;
					for(var i = 0; i < recordedMistakes.length; i++)
					{
						if(recordedMistakes[i].position === absolutePos)
							removeCount++;
						else
							newRecordedMistakes[newRecordedMistakes.length] = recordedMistakes[i];
					}
					exerciseMistakes -= removeCount;
					recordedMistakes = newRecordedMistakes;
					var currentMistake = Qt.createQmlObject("import OpenTyper 1.0; MistakeRecord {}", root);
					currentMistake.position = absolutePos;
					currentMistake.previousText = keyText;
					currentMistake.type = MistakeRecord.Type_Change;
					recordedMistakes[recordedMistakes.length] = currentMistake;
					if(ignoreMistakeAppend)
						paper.mistake = paper.mistake.substring(0, paper.mistake.length - 1);
					var errorAppend = keyText;
					if(keyText === " ")
						errorAppend = "_";
					else if(keyText === "\n")
						errorAppend = "↵";
					paper.mistake += errorAppend;
					paper.input += " ";
					exerciseMistakes++;
					mistake = true;
					var errorWord = StringUtils.wordAt(exerciseText, exercisePos);
					if((errorWord !== "") && (errorWords.indexOf(errorWord) === -1))
						errorWords[errorWords.length] = errorWord;
					deadKeys = 0;
				}
				if(keyID === Qt.Key_Backspace)
					ignoreBackspace = true;
			}
		}
		if(!mistake && ignoreMistakeAppend && !ignoreBackspace)
			paper.mistake += "_";
		if(((displayPos >= displayExercise.length) && correctMistakes) || (currentLine >= lineCount + 1))
		{
			if(currentLine >= lineCount + 1)
			{
				paper.input = paper.input.substring(0, paper.input.length - 1);
				fullInput = fullInput.substring(0, fullInput.length - 1);
			}
			// TODO: Add keyRelease() method
			//keyRelease(event);
			exerciseTimer.stop();
			lastTime = exerciseTimer.elapsed / 1000.0;
			endExercise();
		}
		eventInProgress = false;
	}

	function endExercise() {
		exerciseInProgress = false;
		fullInput.replace("‘", "'");
		displayExercise.replace("‘", "'");
		var validator = Qt.createQmlObject("import OpenTyper 1.0; ExerciseValidator {}", root);
		validator.exerciseText = displayExercise;
		validator.inputText = fullInput;
		validator.clearCharacters();
		for(var i = 0; i < recordedCharacters.length; i++)
			validator.addCharacter(recordedCharacters[i]);
		validator.timed = (currentMode == 1);
		validator.time = lastTime;
		if(correctMistakes)
		{
			validator.clearMistakes();
			for(var j = 0; j < recordedMistakes.length; j++)
				validator.addMistake(recordedMistakes[j]);
			fullInput = validator.textWithMistakes();
			validator.inputText = fullInput;
		}
		else
		{
			validator.validate();
			recordedMistakes = validator.mistakes;
			totalHits = validator.grossHits();
			exerciseMistakes = validator.mistakeCount();
			errorWords = validator.errorWords();
			netHits = Math.max(0, totalHits - (exerciseMistakes * Settings.errorPenalty()));
		}
		validator.generateMistakeText(correctMistakes);
		netHits = Math.max(0, totalHits - exerciseMistakes * Settings.errorPenalty());
		var netHitsPerMinute = netHits * (60 / lastTime);
		var grossHitsPerMinute = totalHits * (60 / lastTime);
		var time = lastTime;
		if(!customExerciseLoaded && !customPack && correctMistakes)
		{
			// TODO: Send end stock exercise event
			/*QVariantMap eventArgs;
			eventArgs["packName"] = publicConfigName;
			eventArgs["lesson"] = currentLesson;
			eventArgs["sublesson"] = currentAbsoluteSublesson;
			eventArgs["exercise"] = currentLevel;
			eventArgs["grossHitsPerMinute"] = grossHitsPerMinute;
			eventArgs["mistakes"] = levelMistakes;
			eventArgs["time"] = time;
			AddonApi::sendEvent(IAddon::Event_EndStockExercise, eventArgs);*/
			// The result will always be saved locally - even if an addon uses it
			var result = [grossHitsPerMinute, exerciseMistakes, time];
			HistoryParser.addHistoryEntry(packName, currentLesson, currentAbsoluteSublesson, currentExercise, result);
		}
		if(testLoaded)
		{
			correctMistakes = true;
			hideText = false;
			// TODO: Restore window geometry if full screen
			// TODO: Send end typing test event
			/*QVariantMap args;
			QList<QVariant> recordedCharactersList;
			for(int i = 0; i < recordedCharacters.count(); i++)
			{
				QVariant record = QVariant::fromValue(recordedCharacters[i]);
				recordedCharactersList.append(record);
			}
			args["recordedCharacters"] = recordedCharactersList;
			args["inputText"] = input;
			args["time"] = lastTimeF;
			AddonApi::sendEvent(IAddon::Event_EndTypingTest, args);*/
			uiLocked = false;
			testLoaded = false;
		}
		summaryDialog.setNetHits(netHitsPerMinute);
		summaryDialog.setGrossHits(grossHitsPerMinute);
		summaryDialog.setTotalTime(time);
		summaryDialog.setTotalHits(totalHits);
		summaryDialog.setMistakes(exerciseMistakes);
		summaryDialog.setAccuracy(1.0 - exerciseMistakes / totalHits);
		summaryDialog.onAccepted.connect(function() {
			changeMode(0);
			// TODO: Show export button
			preview = true;
			// Load saved text
			paper.input = validator.generatedInputText();
			paper.mistake = validator.generatedMistakeText();
			// Hide other widgets
			paper.currentLineVisible = false;
			paper.remainingVisible = false;
			blockInput = true;
		});
		summaryDialog.onRejected.connect(function() {
			changeMode(0);
			if(customExerciseLoaded)
				initExercise();
			else
				repeatExercise();
		});
		summaryDialog.exec();
	}

	function loadText(text, includeNewLines) {
		exerciseText = "";
		var lines =  text.split('\n');
		for(var i = 0; i < lines.length; i++)
		{
			var line = lines[i];
			if(exerciseText == "")
				exerciseText = line;
			else
			{
				if(includeNewLines)
					exerciseText += "\n" + line;
				else
					exerciseText += " " + line;
			}
		}
		customExerciseText = exerciseText;
		customExerciseLoaded = true;
		initExercise();
	}

	function loadTestFinished() {
		// TODO: Send events
		/*AddonApi::setBlockLoadedEx(false);
		QVariantMap args;
		args["loadExDialog"] = QVariant::fromValue((void *) dialog);
		AddonApi::sendEvent(IAddon::Event_CustomExLoaded, args);
		if(!AddonApi::blockLoadedEx())
			initTest(dialog->exerciseText().toUtf8(), dialog->lineLength(), dialog->includeNewLines(),
				dialog->mode(), QTime(0, 0, 0).secsTo(dialog->timeLimit()), dialog->correctMistakes(), dialog->lockUi(), dialog->hideText());*/
		initTest(loadExerciseDialog.exerciseText(), loadExerciseDialog.lineLength(), loadExerciseDialog.includeNewLines(),
			 loadExerciseDialog.mode(), loadExerciseDialog.timeLimitSecs(), loadExerciseDialog.correctMistakes(), loadExerciseDialog.lockUi(), loadExerciseDialog.hideText());
	}

	function initTest(text, lineLength, includeNewLines, mode, time, correctMistakes_, lockUi, hideText_) {
		changeMode(mode);
		exerciseLineLength = lineLength;
		loadText(text, includeNewLines);
		if(mode === 1)
			startTimedExercise(time);
		correctMistakes = correctMistakes_;
		hideText = hideText_;
		if(lockUi)
		{
			// TODO: Save window geometry
			showFullScreen();
			uiLocked = true;
		}
		testLoaded = true;
	}

	function changeMode(mode) {
		currentMode = mode;
		// TODO: Send change mode event
		//AddonApi::sendEvent(IAddon::Event_ChangeMode);
	}

	function startTimedExercise(time)
	{
		timedExTime = time;
		timedExCountdown = 3;
		timedExStarted = false;
		blockInput = true;
		changeMode(1);
		exerciseTimer.start();
		updateTimer.triggered();
		updateTimer.restart();
		repeatExercise();
		exerciseInProgress = true;
	}

	Component.onCompleted: {
		QmlUtils.blurSource = mainLayout;
		loadExerciseDialog.onAccepted.connect(function() { loadTestFinished(); });
		reload();
	}
}
