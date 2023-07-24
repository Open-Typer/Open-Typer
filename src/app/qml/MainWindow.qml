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
import QtCharts 2.3
import OpenTyper 1.0
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0
import OpenTyper.Translations 1.0
import OpenTyper.Global 1.0
import OpenTyper.PackEditor 1.0
import "dialogs"
import "core"

ApplicationWindow {
	property int oldWidth
	property int oldHeight
	property int oldX
	property int oldY
	property bool wasMaximized
	Material.theme: ThemeEngine.theme === ThemeEngine.DarkTheme ? Material.Dark : Material.Light
	Material.accent: ThemeEngine.currentAccentColor
	color: ThemeEngine.bgColor
	minimumWidth: mainLayout.minWidth
	minimumHeight: mainLayout.y - menuBarLine.y + mainLayout.minHeight
	visible: true
	id: root
	onXChanged: geometryTimer.start()
	onYChanged: geometryTimer.start()
	onWidthChanged: geometryTimer.start()
	onHeightChanged: geometryTimer.start()
	onActiveFocusItemChanged: QmlUtils.activeFocusItem = activeFocusItem
	onVisibilityChanged: {
		if(visibility == ApplicationWindow.Maximized)
			wasMaximized = true;
		else if(visibility == ApplicationWindow.Windowed)
		{
			if(!QmlUtils.osMac())
			{
				width = oldWidth;
				height = oldHeight;
				x = oldX;
				y = oldY;
			}
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
			menuList: AppMenuBar.menus
			width: root.width
			enabled: !homeTab.uiLocked
		}
		BackgroundBlur {
			id: menuBarBlur
			anchors.fill: parent
			source: customMenuBar
		}
	}

	MenuBarManager {
		onNewLessonPackToggled: {
			let editor = openPackEditor();
			editor.newFile();
		}

		onOpenPackInEditorToggled: lessonPackFileDialog.getOpenFileContent()
		onAboutProgramToggled: aboutDialog.open()
		onPreferencesToggled: settingsDialog.open()
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

	Rectangle {
		id: menuBarLine
		visible: customMenuBar.visible
		width: parent.width
		implicitHeight: visible ? 1 : 0
		color: ThemeEngine.borderColor
	}

	Rectangle {
		id: tabBarRect
		width: parent.width
		height: tabBar.visible ? tabBar.height : 0
		anchors.top: menuBarLine.bottom
		color: ThemeEngine.panelColor

		CustomTabBar {
			id: tabBar
			hideOnSingleTab: true

			CustomTabButton {
				text: qsTr("Home")
			}
		}

		BackgroundBlur {
			id: tabBarBlur
			anchors.fill: tabBar
			source: tabBar
		}
	}

	StackLayout {
		readonly property Item currentItem: children[currentIndex]
		readonly property int minWidth: currentItem.minWidth ? currentItem.minWidth : implicitWidth
		readonly property int minHeight: currentItem.minHeight ? currentItem.minHeight : implicitHeight
		id: mainLayout
		width: parent.width
		anchors.top: tabBarRect.bottom
		anchors.bottom: parent.bottom
		anchors.topMargin: tabBar.visible ? 1 : 0
		currentIndex: tabBar.currentIndex
		onCurrentIndexChanged: {
			renderSource();

			// Enable or disable home tab menu items
			let isHomeTab = (itemAt(currentIndex) === homeTab);

			AppMenuBar.openExerciseAction.enabled = isHomeTab;
			AppMenuBar.openPackAction.enabled = isHomeTab;
			AppMenuBar.printAction.enabled = isHomeTab;

			AppMenuBar.typingTestAction.enabled = isHomeTab;

			AppMenuBar.exerciseHistoryAction.enabled = isHomeTab;
			AppMenuBar.timedExAction.enabled = isHomeTab;
			AppMenuBar.errorWordsAction.enabled = isHomeTab;
			AppMenuBar.reverseTextAction.enabled = isHomeTab;
		}

		function renderSource() {
			mainLayoutSource.render();
		}

		Home {
			id: homeTab
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
		onSettingsSynced: homeTab.reload()
	}

	AboutDialog {
		id: aboutDialog
	}

	QmlFileDialog {
		id: lessonPackFileDialog
		nameFilters: [qsTr("Open-Typer pack files") + "(*.typer)"]
		onFileContentReady: {
			let editor = openPackEditor();
			editor.fileName = fileName;
		}
	}

	Component {
		id: packEditorComponent

		PackEditor {}
	}

	Component {
		id: packEditorTabButtonComponent

		CustomTabButton {
			property PackEditor editor
			id: editorButton
			text: FileUtils.fileName(editor.fileName) + (editor.saved ? "" : "*")
			closable: true
			onClosed: {
				let index = -1;

				for(var i = 0; i < tabBar.contentChildren.length; i++)
				{
					if(tabBar.contentChildren[i] == this)
					{
						index = i;
						break;
					}
				}

				console.assert(index != -1);
				tabBar.currentIndex = index;
				editor.close()
			}

			Connections {
				target: editorButton.editor
				onAboutToClose: tabBar.removeItem(editorButton);
			}
		}
	}

	function openPackEditor() {
		let editorObject = packEditorComponent.createObject(mainLayout);
		let editorButton = packEditorTabButtonComponent.createObject(tabBar, { editor: editorObject });
		tabBar.currentIndex = tabBar.count - 1;

		return editorObject;
	}

	Component.onCompleted: {
		visibility = Settings.getValue("app", "windowMaximized") ? ApplicationWindow.Maximized : ApplicationWindow.Windowed;
		wasMaximized = (visibility == ApplicationWindow.Maximized);
		oldX = Settings.getValue("app", "windowX");
		oldY = Settings.getValue("app", "windowY");
		oldWidth = Settings.getValue("app", "windowWidth");
		oldHeight = Settings.getValue("app", "windowHeight");
		if(!Settings.containsKey("app", "windowX") || !Settings.containsKey("app", "windowY"))
		{
			oldX = (screen.width - oldWidth) / 2;
			oldY = (screen.height - oldHeight) / 2;
		}
		showNormal();
		x = oldX;
		y = oldY;
		width = oldWidth;
		height = oldHeight;
		if(Settings.getValue("app", "windowMaximized"))
		{
			if(QmlUtils.osWindows() && (QmlUtils.qtVersionMajor() < 6))
				showFullScreen();
			showMaximized();
		}
		QmlUtils.blurSource = mainLayout;
		QmlUtils.bgBlur = bgBlur;
		QmlUtils.menuBarBlur = menuBarBlur;
		QmlUtils.tabBarBlur = tabBarBlur;
		QmlUtils.activeFocusItem = root.activeFocusItem;
		if(!Settings.getValue("app", "initFinished"))
			initialSetup.open();
		homeTab.reload();
		homeTab.paper.forceActiveFocus();
	}

	onClosing: {
		// Close pack editor tabs
		if(tabBar.count > 1)
		{
			close.accepted = false;
			tabBar.itemAt(1).editor.onAboutToClose.connect(function() { root.close() });
			tabBar.itemAt(1).closed();
		}

		// Save window geometry
		if(visibility != ApplicationWindow.Windowed)
		{
			Settings.setValue("app", "windowX", oldX);
			Settings.setValue("app", "windowY", oldY);
			Settings.setValue("app", "windowWidth", oldWidth);
			Settings.setValue("app", "windowHeight", oldHeight);
		}
		else
		{
			Settings.setValue("app", "windowX", root.x);
			Settings.setValue("app", "windowY", root.y);
			Settings.setValue("app", "windowWidth", root.width);
			Settings.setValue("app", "windowHeight", root.height);
		}
		Settings.setValue("app", "windowMaximized", wasMaximized);
	}
}
