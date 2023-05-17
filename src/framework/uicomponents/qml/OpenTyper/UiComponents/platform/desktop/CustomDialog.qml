/*
 * CustomDialog.qml
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
import QtQuick.Controls.Material 2.5
import QtQuick.Layouts 1.12
import OpenTyper.UiComponents 1.0
import OpenTyper.Ui 1.0
import OpenTyper.Translations 1.0
import "internal"

Item {
	id: root

	property Component contentItem: Item {}
	readonly property alias contents: dialog.contents
	property int standardButtons
	property string title: Qt.application.displayName
	property bool fixedSize: true
	property bool maximized: false
	signal accepted()
	signal applied()
	signal discarded()
	signal helpRequested()
	signal rejected()
	signal reset()
	signal opened()
	signal closed()

	visible: dialog.visible
	onVisibleChanged: {
		dialog.visible = visible;
		if(visible)
			opened();
		else
			closed();
		visible = Qt.binding(function() { return dialog.visible });
	}
	onMaximizedChanged: {
		if(maximized)
			dialog.setMaximizedState();
		else
			dialog.setNormalState();
	}

	function open() {
		visible = true;
	}

	function close() {
		visible = false;
	}

	function accept() {
		close();
		accepted();
	}

	function reject() {
		close();
		rejected();
	}

	DialogView {
		readonly property Item contents: contentItem.contentsLoader.item
		id: dialog
		title: root.title
		visible: false
		minimumWidth: contentItem.contentLayout.implicitWidth
		minimumHeight: contentItem.contentLayout.implicitHeight
		maximumWidth: {
			if(fixedSize)
				return minimumWidth;
		}
		maximumHeight: {
			if(fixedSize)
				return minimumHeight;
		}

		contentItem: Rectangle {
			property alias contentLayout: contentLayout
			property alias contentsLoader: contentsLoader
			anchors.fill: parent
			color: ThemeEngine.bgColor
			Material.background: ThemeEngine.bgColor
			Material.accent: ThemeEngine.currentAccentColor
			Material.theme: ThemeEngine.theme === ThemeEngine.DarkTheme ? Material.Dark : Material.Light
			ColumnLayout {
				id: contentLayout
				anchors.fill: parent

				Loader {
					id: contentsLoader
					sourceComponent: root.contentItem
					Layout.fillWidth: true
					Layout.fillHeight: true
				}

				Loader {
					id: buttonBoxLoader
					sourceComponent: CustomDialogButtonBox {
						standardButtons: root.standardButtons
						onAccepted: root.accept()
						onApplied: root.applied()
						onDiscarded: root.discarded()
						onHelpRequested: root.helpRequested()
						onRejected: root.reject()
						onReset: root.reset()
						onFocusOut: dialog.contentItem.forceActiveFocus(Qt.TabFocus)
					}
					Layout.fillWidth: true
					Connections {
						target: LanguageManager
						onLanguageChanged: {
							translationTimer.start();
						}
					}
					Timer {
						id: translationTimer
						interval: 16
						running: false
						repeat: false
						onTriggered: {
							buttonBoxLoader.active = 0;
							buttonBoxLoader.active = 1;
						}
					}
				}
			}
		}
	}

	Component.onCompleted: maximizedChanged()
}
