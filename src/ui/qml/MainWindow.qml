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
import "core"

ApplicationWindow {
	property int oldWidth
	property int oldHeight
	property int oldX
	property int oldY
	property bool wasMaximized
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
	readonly property alias keyboard: paper.keyboard
	Material.theme: ThemeEngine.theme === ThemeEngine.DarkTheme ? Material.Dark : Material.Light
	Material.accent: ThemeEngine.currentAccentColor
	color: ThemeEngine.bgColor
	minimumWidth: mainLayout.minWidth
	minimumHeight: mainLayout.minHeight
	visible: true
	id: root
	onXChanged: geometryTimer.start()
	onYChanged: geometryTimer.start()
	onWidthChanged: geometryTimer.start()
	onHeightChanged: geometryTimer.start()
	onVisibilityChanged: {
		if(visibility == ApplicationWindow.Maximized)
			wasMaximized = true;
		else if(visibility == ApplicationWindow.Windowed)
		{
			width = oldWidth;
			height = oldHeight;
			x = oldX;
			y = oldY;
			wasMaximized = false;
		}
	}

	Timer {
		id: geometryTimer
		interval: 16
		running: false
		repeat: false
		onTriggered: {
			if(visibility == ApplicationWindow.Windowed)
			{
				oldWidth = width;
				oldHeight = height;
				oldX = x;
				oldY = y;
			}
			mainLayoutSource.render();
		}
	}

	menuBar: Item {
		width: customMenuBar.width
		height: customMenuBar.height
		CustomMenuBar {
			id: customMenuBar
			width: root.width
			enabled: !uiLocked
		}
		BackgroundBlur {
			id: menuBarBlur
			anchors.fill: parent
			source: customMenuBar
		}
	}

	MenuBarManager {
		onOpenToggled: panel1.contents.openButton.clicked()
		onPrintToggled: panel1.contents.printButton.clicked()
		onTypingTestToggled: panel1.contents.typingTestButton.clicked()
		onExerciseHistoryToggled: panel2.contents.statsButton.checked = true
		onTimedExToggled: panel1.contents.timedExButton.clicked()
		onErrorWordsToggled: panel1.contents.errorWordsButton.clicked()
		onReverseTextToggled: panel1.contents.reverseTextButton.clicked()
		onPreferencesToggled: settingsDialog.open()
		onAboutProgramToggled: aboutDialog.open()
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
		onLanguageChanged: {
			updateLessonList();
			panel2.contents.lessonBox.currentIndex = currentLesson - 1;
			loadLesson(currentLesson, currentSublesson);
			loadSublesson(currentExercise);
		}
	}

	Connections {
		target: QmlUtils
		onScreenKeyboardChanged: {
			if(layoutChanged)
				keyboard.loadLayout();
		}
	}

	ShaderEffectSource {
		id: mainLayoutSource
		anchors.fill: mainLayout
		sourceItem: mainLayout
		live: false

		function render() {
			scheduleUpdate();
		}

		Connections {
			target: root
			onWidthChanged: { mainLayoutSource.render(); }
			onHeightChanged: { mainLayoutSource.render(); }
			onPreviewChanged: { mainLayoutSource.render(); }
			onCurrentModeChanged: { mainLayoutSource.render(); }
		}

		Connections {
			target: paper.paperRect
			onWidthChanged: { mainLayoutSource.render(); }
		}

		Connections {
			target: ThemeEngine
			onCurrentAccentColorChanged: { mainLayoutSource.render(); }
		}

		Connections {
			target: LanguageManager
			onLanguageChanged: { mainLayoutSource.render(); }
		}
	}

	DropShadow {
		source: mainLayoutSource
		anchors.fill: mainLayoutSource
		horizontalOffset: 0
		verticalOffset: 5
		radius: 17
		samples: 13
		cached: true
		color: ThemeEngine.theme === ThemeEngine.DarkTheme ? "#80000000" : "#80000022"
		visible: true
	}

	ColumnLayout {
		property int minWidth: Math.max(implicitWidth, paper.paperRect.width)
		property int minHeight: implicitHeight
		id: mainLayout
		anchors.fill: parent
		spacing: 0
		onFocusChanged: {
			if(focus)
				panel1.contents.settingsButton.forceActiveFocus(Qt.TabFocus);
		}
		Keys.forwardTo: keyboardHandler

		KeyboardHandler {
			id: keyboardHandler
			onKeyPressed: {
				if(event["key"] !== Qt.Key_Tab && event["key"] !== Qt.Key_Enter && event["key"] !== Qt.Key_Return && event["key"] !== Qt.Key_Space)
					paper.forceActiveFocus();
			}
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
				readonly property alias settingsButton: settingsButton
				readonly property alias openButton: openButton
				readonly property alias printButton: printButton
				readonly property alias typingTestButton: typingTestButton
				readonly property alias timedExButton: timedExButton
				readonly property alias errorWordsButton: errorWordsButton
				readonly property alias reverseTextButton: reverseTextButton
				CustomToolButton {
					id: settingsButton
					icon.name: "settings"
					toolTipText: qsTr("Options")
					onClicked: settingsDialog.open()
				}
				CustomToolButton {
					id: openButton
					icon.name: "open"
					toolTipText: qsTr("Open")
					onClicked: customExFileDialog.getOpenFileContent()
				}
				CustomToolButton {
					id: printButton
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
				AddonButtonRepeater { model: AddonApi.mainButtons }
				ToolSeparator {}
				CustomToolButton {
					id: typingTestButton
					icon.name: "paper"
					text: qsTr("Typing test")
					onClicked: {
						typingTestDialog.open();
					}
				}
				CustomToolButton {
					id: timedExButton
					icon.name: "time"
					text: qsTr("Timed exercise")
					onClicked: timeDialog.open()
				}
				CustomToolButton {
					id: errorWordsButton
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
					id: reverseTextButton
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
				AddonButtonRepeater { model: AddonApi.exOptionsButtons }
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
					accessibleDescription: toolTipText + " " + qsTr("Start typing")
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
				AddonButtonRepeater { model: AddonApi.navigationButtons }
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
				AddonButtonRepeater { model: AddonApi.exInfoButtons }
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
			id: appUpdateQuestion
			visible: false
			Layout.fillWidth: true
			onAccepted: Updater.installUpdate()
			Component.onCompleted: {
				if(Updater.updateAvailable())
					visible = true;
			}
			onOpacityChanged: mainLayoutSource.render()
			onVisibleChanged: mainLayoutSource.render()
		}
		UpdateQuestion {
			id: addonUpdateQuestion
			visible: false
			addons: true
			Layout.fillWidth: true
			onAccepted: Updater.updateAddons()
			Component.onCompleted: {
				if(Updater.addonUpdateAvailable())
					visible = true;
			}
			onOpacityChanged: mainLayoutSource.render()
			onVisibleChanged: mainLayoutSource.render()
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
		}
	}

	BackgroundBlur {
		id: bgBlur
		anchors.fill: mainLayout
		// TODO: Use mainLayout as the source after removing QmlUtils.blurSource
		source: QmlUtils.blurSource
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

	AboutDialog {
		id: aboutDialog
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
		AddonApi.sendEvent(AddonApi.Event_RefreshApp);
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
		if(!customExerciseLoaded)
		{
			correctMistakes = true;
			hideText = false;
		}
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
		// Reload menu bar
		QmlUtils.reloadMenuBar();
		AddonApi.sendEvent(AddonApi.Event_InitExercise);
	}

	function updateText() {
		paper.currentLineVisible = true;
		paper.remainingVisible = true;
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
		paper.forceActiveFocus();
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
		validator.targetHitsPerMinute = ClassManager.targetHitsPerMinute();
		if(correctMistakes)
		{
			validator.clearMistakes();
			for(var j = 0; j < recordedMistakes.length; j++)
				validator.addMistake(recordedMistakes[j]);
			fullInput = validator.textWithMistakes();
			validator.inputText = fullInput;
			validator.validate(totalHits, errorWords);
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
			var eventArgs = {
				"packName": packName,
				"lesson": currentLesson,
				"sublesson": currentAbsoluteSublesson,
				"exercise": currentExercise,
				"grossHitsPerMinute": grossHitsPerMinute,
				"mistakes": exerciseMistakes,
				"time": time
			};
			AddonApi.sendEvent(AddonApi.Event_EndStockExercise, eventArgs);
			// The result will always be saved locally - even if an addon uses it
			historyParser.append(grossHitsPerMinute, exerciseMistakes, time);
		}
		if(testLoaded)
		{
			correctMistakes = true;
			hideText = false;
			if(visibility == ApplicationWindow.FullScreen)
			{
				if(wasMaximized)
					showMaximized();
				else
					showNormal();
			}
			var args = {
				"recordedCharacters": recordedCharacters,
				"inputText": fullInput,
				"time": time
			};
			AddonApi.sendEvent(AddonApi.Event_EndTypingTest, args);
			uiLocked = false;
			testLoaded = false;
		}
		paper.summary.netHits = netHitsPerMinute;
		paper.summary.grossHits = grossHitsPerMinute;
		paper.summary.totalTime = time;
		paper.summary.totalHits = totalHits;
		paper.summary.mistakes = exerciseMistakes;
		paper.summary.accuracy = 1.0 - exerciseMistakes / totalHits;
		paper.summary.validator = validator;
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
		// Reload menu bar
		QmlUtils.reloadMenuBar();
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
		AddonApi.sendEvent(AddonApi.Event_CustomExLoaded);
		initTest(typingTestDialog.exerciseText, parser.defaultLineLength(), true, (typingTestDialog.timed ? 1 : 0),
			 typingTestDialog.timeLimitSecs, typingTestDialog.correctMistakes, typingTestDialog.lockUi, typingTestDialog.hideText);
	}

	function initTest(text, lineLength, includeNewLines, mode, time, correctMistakes_, lockUi, hideText_) {
		changeMode(mode);
		exerciseLineLength = lineLength;
		correctMistakes = correctMistakes_;
		hideText = hideText_;
		loadText(text, includeNewLines);
		if(mode === 1)
			startTimedExercise(time);
		if(lockUi)
		{
			showFullScreen();
			uiLocked = true;
		}
		testLoaded = true;
	}

	function changeMode(mode) {
		currentMode = mode;
		AddonApi.sendEvent(AddonApi.Event_ChangeMode);
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
		visibility = Settings.windowMaximized() ? ApplicationWindow.Maximized : ApplicationWindow.Windowed;
		wasMaximized = (visibility == ApplicationWindow.Maximized);
		oldX = Settings.windowX();
		oldY = Settings.windowY();
		oldWidth = Settings.windowWidth();
		oldHeight = Settings.windowHeight();
		if(!Settings.containsWindowX() || !Settings.containsWindowY())
		{
			oldX = (screen.width - oldWidth) / 2;
			oldY = (screen.height - oldHeight) / 2;
		}
		showNormal();
		x = oldX;
		y = oldY;
		width = oldWidth;
		height = oldHeight;
		if(Settings.windowMaximized())
		{
			if(QmlUtils.osWindows() && (QmlUtils.qtVersionMajor() < 6))
				showFullScreen();
			showMaximized();
		}
		QmlUtils.blurSource = mainLayout;
		QmlUtils.bgBlur = bgBlur;
		QmlUtils.menuBarBlur = menuBarBlur;
		QmlUtils.activeFocusItem = Qt.binding(function() { return root.activeFocusItem; });
		AddonApi.sendEvent(AddonApi.Event_InitApp);
		if(!Settings.initFinished())
			initialSetup.open();
		reload();
		paper.forceActiveFocus();
	}

	onClosing: {
		if(visibility != ApplicationWindow.Windowed)
		{
			Settings.setWindowX(oldX);
			Settings.setWindowY(oldY);
			Settings.setWindowWidth(oldWidth);
			Settings.setWindowHeight(oldHeight);
		}
		else
		{
			Settings.setWindowX(root.x);
			Settings.setWindowY(root.y);
			Settings.setWindowWidth(root.width);
			Settings.setWindowHeight(root.height);
		}
		Settings.setWindowMaximized(wasMaximized);
	}
}
