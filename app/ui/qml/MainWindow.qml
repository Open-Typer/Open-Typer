/*
 * MainWindow.qml
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
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.5
import QtGraphicalEffects 1.0
import Qt5Compat.GraphicalEffects 1.0
import OpenTyper 1.0
import "controls"
import "dialogs"

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
	property bool blockNextDeadKey: false
	property string lastDeadKey
	property string lastKey
	property bool testLoaded: false
	property bool uiLocked: false
	property string formattedExerciseTime
	property string formattedExerciseRemainingTime
	Material.theme: ThemeEngine.theme === ThemeEngine.DarkTheme ? Material.Dark : Material.Light
	Material.accent: ThemeEngine.currentAccentColor
	color: ThemeEngine.bgColor
	minimumWidth: mainLayout.minWidth
	minimumHeight: mainLayout.minHeight
	// TODO: Load window geometry from settings
	width: 1200
	height: 800
	visible: true
	id: root

	menuBar: Item {
		width: customMenuBar.width
		height: customMenuBar.height
		CustomMenuBar {
			id: customMenuBar
			width: root.width
			enabled: !uiLocked
		}
		FastBlur {
			readonly property alias blurInAnimation: menuBarBlurInAnimation
			readonly property alias blurOutAnimation: menuBarBlurOutAnimation
			id: menuBarBlur
			anchors.fill: parent
			source: customMenuBar
			visible: false
			radius: 0
			PropertyAnimation {
				id: menuBarBlurInAnimation
				target: menuBarBlur
				property: "radius"
				to: 40
				duration: 125
			}
			PropertyAnimation {
				id: menuBarBlurOutAnimation
				target: menuBarBlur
				property: "radius"
				to: 0
				duration: 125
			}
		}
	}

	ConfigParser {
		id: parser
	}

	HistoryParser {
		id: historyParser
		lessonPack: packName
		lesson: currentLesson
		sublesson: currentAbsoluteSublesson
		exercise: currentExercise
		onCountChanged: paper.historyChanged()
	}

	ExerciseTimer {
		id: exerciseTimer
	}

	Timer {
		id: updateTimer
		interval: 16
		repeat: true
		running: exerciseInProgress
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

	Connections {
		target: rootItem
		function onLanguageChanged() {
			updateLessonList();
			panel2.contents.lessonBox.currentIndex = currentLesson - 1;
			loadLesson(currentLesson, currentSublesson);
			loadSublesson(currentExercise);
		}
	}

	Connections {
		target: QmlUtils
		function onScreenKeyboardChanged(layoutChanged) {
			if(layoutChanged)
				keyboard.loadLayout();
		}
	}

	Repeater {
		readonly property int minY: timedExPanel.visible ? 0 : panel1.height
		id: shadowRepeater
		model: timedExPanel.visible ? [timedExPanel, paper] : [panel2, exportButton, paper]
		DropShadow {
			function getY() {
				var out = shadowRepeater.minY;
				for(var i = 0; i < shadowRepeater.model.length; i++)
				{
					if(shadowRepeater.model[i] === paper)
						out += paper.Layout.topMargin;
					if(i === index)
						return out;
					if(shadowRepeater.model[i].visible)
						out += shadowRepeater.model[i].height;
				}
				return out;
			}
			source: modelData
			width: modelData.width
			height: modelData.height
			y: getY()
			horizontalOffset: 0
			verticalOffset: 5
			radius: 17
			samples: 13
			color: ThemeEngine.theme === ThemeEngine.DarkTheme ? "#80000000" : "#80000022"
			visible: modelData.visible && modelData != exportButton
		}
	}

	ColumnLayout {
		property int minWidth: Math.max(implicitWidth, paper.paperRect.width)
		property int minHeight: implicitHeight
		id: mainLayout
		anchors.fill: parent
		spacing: 0

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
					icon.name: "settings"
					toolTipText: qsTr("Options")
					onClicked: settingsDialog.open()
				}
				CustomToolButton {
					icon.name: "open"
					toolTipText: qsTr("Open")
					onClicked: customExFileDialog.getOpenFileContent()
				}
				CustomToolButton {
					icon.name: "print"
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
					icon.name: "paper"
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
						typingTestDialog.open();
					}
				}
				CustomToolButton {
					icon.name: "time"
					text: qsTr("Timed exercise")
					onClicked: timeDialog.open()
				}
				CustomToolButton {
					icon.name: "close"
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
					icon.name: "rewind"
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
					icon.name: "repeat"
					toolTipText: qsTr("Repeat exercise")
					onClicked: repeatExerciseClicked();
				}
				CustomToolButton {
					id: closeLoadedExButton
					icon.name: "close"
					toolTipText: qsTr("Close loaded exercise")
					onClicked: {
						customExerciseLoaded = false
						repeatExercise();
					}
				}
				CustomToolButton {
					icon.name: "left"
					toolTipText: qsTr("Previous exercise")
					onClicked: previousExercise();
				}
				CustomToolButton {
					icon.name: "right"
					toolTipText: qsTr("Next exercise")
					onClicked: nextExercise();
				}
				CustomComboBox { id: lessonBox; onActivated: selectLesson(index); }
				CustomComboBox { id: sublessonBox; onActivated: selectSublesson(index); }
				CustomComboBox { id: exerciseBox; onActivated: selectExercise(index); }
				ToolSeparator {}
				CustomToolButton {
					id: statsButton
					icon.name: "stats"
					text: qsTr("Exercise history")
					checkable: true
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
					icon.name: "close"
					toolTipText: qsTr("Stop")
					onClicked: {
						changeMode(0);
						repeatExercise();
					}
				}
				Item { Layout.fillWidth: true }
			}
		}
		UpdateQuestion {
			visible: false
			Layout.fillWidth: true
			onAccepted: Updater.installUpdate()
			Component.onCompleted: {
				if(Updater.updateAvailable())
					visible = true;
			}
		}
		AccentButton {
			id: exportButton
			Layout.alignment: Qt.AlignHCenter
			text: qsTr("Export")
			icon.name: "download"
			visible: preview
			onClicked: exportDialog.open()
		}
		Paper {
			id: paper
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.topMargin: 10
			summary.visible: preview
			exerciseHistory: panel2.contents.statsButton.checked
			blockInput: root.blockInput
			lessonPack: packName
			lesson: root.currentLesson
			sublesson: root.currentAbsoluteSublesson
			exercise: root.currentExercise
			onKeyPressed: keyPress(event);
			onKeyReleased: keyRelease(event);
			onExerciseHistoryChanged: {
				if(exerciseHistory)
				{
					showKeyboardAnimation.stop();
					hideKeyboardAnimation.start();
				}
				else
				{
					hideKeyboardAnimation.stop();
					showKeyboardAnimation.start();
				}
			}

			PropertyAnimation {
				id: showKeyboardAnimation
				target: keyboard
				property: "scale"
				to: 1
				duration: 125
				easing.type: Easing.OutCubic
			}

			PropertyAnimation {
				id: hideKeyboardAnimation
				target: keyboard
				property: "scale"
				to: 0
				duration: 125
				easing.type: Easing.InCubic
			}

			KeyboardView {
				id: keyboard
				visible: !preview
				anchors.bottom: parent.bottom
				anchors.bottomMargin: 20
				x: parent.width / 2 - width / 2
			}
		}
	}

	InitialSetup {
		id: initialSetup
		onAboutToHide: reload()
	}

	SettingsDialog {
		id: settingsDialog
		onSettingsSynced: reload()
	}

	MessageBox {
		id: largeFileBox
		windowTitle: qsTr("Error");
		title: qsTr("This file is too large!")
		icon: MessageBox.Critical
	}

	TypingTestDialog {
		id: typingTestDialog
		parserObj: parser
		onAccepted: loadTestFinished()
	}

	TimeDialog {
		id: timeDialog
		onAccepted: startTimedExercise(timeSecs)
	}

	MessageBox {
		id: noErrorWordsBox
		windowTitle: qsTr("No error words");
		title: qsTr("You don't have any error words.")
		icon: MessageBox.Information
	}

	ExportDialog {
		id: exportDialog
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
		// Load on screen keyboard layout
		keyboard.loadLayout();
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
		paper.errorPenalty = Settings.errorPenalty();
		highlightNextKey();
		keyboard.releaseAllKeys();
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
		highlightNextKey();
		keyboard.releaseAllKeys();
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

	function repeatExerciseClicked() {
		repeatExercise();
		panel2.contents.statsButton.checked = false;
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
		var isDeadKey = KeyboardUtils.isDeadKey(keyID);
		if(!isDeadKey)
		{
			if(deadKeys > 0)
			{
				keyboard.releaseAllKeys();
				if(lastDeadKey === event["text"])
					blockNextDeadKey = true;
			}
			else
				blockNextDeadKey = false;
			keyboard.pressKey(event);
		}
		if(event["isAutoRepeat"])
			return;
		if(isDeadKey)
		{
			if(blockNextDeadKey)
			{
				keyboard.releaseAllKeys();
				blockNextDeadKey = false;
				return;
			}
			deadKeys++;
			lastDeadKey = KeyboardUtils.deadKeyToReadableString(event["key"]);
			// Count modifier key used with the dead key
			if(event["modifiers"] !== Qt.NoModifier)
				deadKeys++;
			event["text"] = KeyboardUtils.deadKeyToString(event["key"]);
			keyboard.pressKey(event);
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
		var rawKeyText = keyText;
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
				var charRecord = QmlUtils.createCharacterRecord();
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
					var currentMistake = QmlUtils.createMistakeRecord();
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
					keyboard.dehighlightAllKeys();
					keyboard.highlightKey({ "key": Qt.Key_Backspace });
				}
				if(keyID === Qt.Key_Backspace)
					ignoreBackspace = true;
			}
		}
		if(!mistake && ignoreMistakeAppend && !ignoreBackspace)
			paper.mistake += "_";
		if(!mistake && correctMistakes)
			highlightNextKey();
		lastKey = rawKeyText;
		if(((displayPos >= displayExercise.length) && correctMistakes) || (currentLine >= lineCount + 1))
		{
			if(currentLine >= lineCount + 1)
			{
				paper.input = paper.input.substring(0, paper.input.length - 1);
				fullInput = fullInput.substring(0, fullInput.length - 1);
			}
			keyRelease(event);
			exerciseTimer.stop();
			lastTime = exerciseTimer.elapsed / 1000.0;
			endExercise();
		}
		eventInProgress = false;
	}

	function keyRelease(event) {
		keyboard.releaseKey(event);
		if(lastKey !== event["text"])
		{
			event["text"] = lastKey;
			keyboard.releaseKey(event);
		}
	}

	function endExercise() {
		exerciseInProgress = false;
		fullInput.replace(/‘/g, "'");
		displayExercise.replace(/‘/g, "'");
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
			historyParser.append(grossHitsPerMinute, exerciseMistakes, time);
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
		paper.summary.netHits = netHitsPerMinute;
		paper.summary.grossHits = grossHitsPerMinute;
		paper.summary.totalTime = time;
		paper.summary.totalHits = totalHits;
		paper.summary.mistakes = exerciseMistakes;
		paper.summary.accuracy = 1.0 - exerciseMistakes / totalHits;
		changeMode(0);
		exportDialog.validator = validator;
		preview = true;
		// Load saved text
		paper.input = validator.generatedInputText();
		paper.mistake = validator.generatedMistakeText();
		// Hide other items
		paper.currentLineVisible = false;
		paper.remainingVisible = false;
		blockInput = true;
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
		exerciseText = exerciseText.replace(/\t/g, " ");
		customExerciseText = exerciseText;
		customExerciseLoaded = true;
		panel2.contents.statsButton.checked = false;
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
		initTest(typingTestDialog.exerciseText, parser.defaultLineLength(), true, (typingTestDialog.timed ? 1 : 0),
			 typingTestDialog.timeLimitSecs, typingTestDialog.correctMistakes, typingTestDialog.lockUi, typingTestDialog.hideText);
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

	function highlightNextKey() {
		if(fullInput.length < exerciseText.length)
		{
			keyboard.dehighlightAllKeys();
			var keys = keyboard.layout.characterKeys(displayExercise[displayPos]);
			for(var i = 0; i < keys.length; i++)
			{
				var futureEvent;
				if(keys[i].type === KeyboardUtils.KeyType_Any)
					futureEvent = { "text": keys[i].displayText };
				else if(keys[i].type === KeyboardUtils.KeyType_RShift)
					futureEvent = { "text": "", "key": Qt.Key_Shift, "rShift": true };
				else if(keys[i].type === KeyboardUtils.KeyType_LShift)
					futureEvent = { "text": "", "key": Qt.Key_Shift };
				else
					futureEvent = { "text": "\n", "key": Qt.Key_Return };
				keyboard.highlightKey(futureEvent);
			}
		}
	}

	Component.onCompleted: {
		QmlUtils.blurSource = mainLayout;
		QmlUtils.menuBarBlur = menuBarBlur;
		if(!Settings.initFinished())
			initialSetup.open();
		reload();
	}
}
