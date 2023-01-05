/*
 * InitialSetup.qml
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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import OpenTyper 1.0
import "../controls"
import "../settings"
import ".."

CustomDialog {
	readonly property var pages: [ localizationPage, lessonPackPage, appearancePage ]
	property int currentIndex: 0
	property int keyboardLayout: -1
	property string keyboardLayoutName : ""
	property bool noAutoLayout: false
	property bool lastOperationNext
	id: root
	//: Application Setup, for example Open-Typer Setup
	windowTitle: qsTr("%1 Setup").arg(Qt.application.displayName)
	draggable: false
	fillWindow: true
	dialog.closePolicy: Popup.NoAutoClose
	function previousPage() {
		if(currentIndex > 0)
		{
			lastOperationNext = false;
			currentIndex--;
			contentItem.stack.replace(contentItem.currentItem, pages[currentIndex], StackView.PopTransition);
		}
	}
	function nextPage() {
		if(currentIndex < pages.length - 1)
		{
			lastOperationNext = true;
			currentIndex++;
			contentItem.stack.replace(contentItem.stack.currentItem, pages[currentIndex], StackView.PushTransition);
		}
	}
	function skipPage() { skipPageTimer.start(); }
	onAboutToShow: {
		if(!Settings.containsSimpleThemeId())
			ThemeEngine.setDefaultTheme();
		if(!Settings.containsLessonPack() || (Settings.lessonPack() === ""))
		{
			Settings.setLessonPack("en_US-default-A"); // lesson pack will be changed during initial setup anyway
			noAutoLayout = true;
		}
	}
	contentComponent: ColumnLayout {
		property alias stack: stack
		anchors.fill: parent
		StackView {
			id: stack
			initialItem: pages[0]
			Layout.fillWidth: true
			Layout.fillHeight: true
		}
		MenuSeparator { Layout.fillWidth: true }
		RowLayout {
			Layout.fillWidth: true
			CustomToolButton {
				visible: currentIndex > 0
				//: %1 is the left arrow
				text: qsTr("%1 Previous").arg("◀")
				foregroundColor: Material.accent
				onClicked: previousPage()
			}
			Item { Layout.fillWidth: true }
			CustomToolButton {
				visible: (currentIndex < pages.length - 1)
				enabled: stack.currentItem.finished
				//: %1 is the right arrow
				text: qsTr("Next %1").arg("▶")
				foregroundColor: Material.accent
				onClicked: nextPage()
			}
			CustomToolButton {
				visible: currentIndex == pages.length - 1
				enabled: stack.currentItem.finished
				//: To finish the initial setup
				text: qsTr("Finish")
				foregroundColor: Material.accent
				onClicked: {
					Settings.setInitFinished(true);
					root.close();
				}
			}
		}
	}
	Timer {
		id: skipPageTimer
		repeat: false
		interval: 10
		onTriggered: {
			if(lastOperationNext)
				nextPage();
			else
				previousPage();
		}
	}
	Component {
		id: localizationPage
		RowLayout {
			property bool finished: layoutList.currentIndex != -1
			ColumnLayout {
				Layout.fillWidth: true
				clip: true
				Pane {
					implicitWidth: 0
					Label {
						anchors.fill: parent
						//: The language for the user interface
						text: qsTr("User interface language")
						font.bold: true
						font.pointSize: 14
					}
				}
				LanguageList {
					Layout.fillWidth: true
					Layout.fillHeight: true
				}
			}
			ToolSeparator { Layout.fillHeight: true }
			ColumnLayout {
				Layout.fillWidth: true
				clip: true
				Pane {
					implicitWidth: 0
					Label {
						anchors.fill: parent
						text: qsTr("Keyboard layout")
						font.bold: true
						font.pointSize: 14
					}
				}
				KeyboardLayoutList {
					id: layoutList
					Layout.fillWidth: true
					Layout.fillHeight: true
					noAutoLayout: root.noAutoLayout
					onCurrentIndexChanged: {
						keyboardLayout = currentIndex;
						keyboardLayoutName = currentItem.text;
					}
					onItemsLoaded: currentIndex = keyboardLayout
				}
			}
		}
	}
	Component {
		id: lessonPackPage
		ColumnLayout {
			property bool finished: packList.currentIndex != -1
			Label {
				text: qsTr("Lesson pack")
				font.bold: true
				font.pointSize: 14
			}
			LessonPackList {
				id: packList
				Layout.fillWidth: true
				Layout.fillHeight: true
				keyboardLayout: root.keyboardLayoutName
				onItemsLoaded: {
					if(items.length == 1)
					{
						currentIndex = 0;
						currentItem.clicked();
						skipPage();
					}
				}
			}
		}
	}
	Component {
		id: appearancePage
		ColumnLayout {
			property bool finished: true
			AppearanceSettings {
				Layout.fillWidth: true
				Layout.alignment: Qt.AlignTop
				simple: true
			}
		}
	}
}
