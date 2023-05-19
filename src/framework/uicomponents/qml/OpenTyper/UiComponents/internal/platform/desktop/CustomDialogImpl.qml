/*
 * CustomDialogImpl.qml
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

Item {
	id: root

	property Component contentItem: Item {}
	readonly property alias contents: dialog.contents
	property int standardButtons
	property string title: Qt.application.displayName
	property bool fixedSize: true
	property bool maximized: false
	property bool autoClose: true
	property int nativeDialogMinimumWidth: dialog.contentItem.contentLayout.implicitWidth
	property int nativeDialogMinimumHeight: dialog.contentItem.contentLayout.implicitHeight
	readonly property bool isNative: true
	property bool closable: true
	signal accepted()
	signal applied()
	signal discarded()
	signal helpRequested()
	signal rejected()
	signal reset()
	signal opened()
	signal closed()
	signal aboutToShow()
	signal aboutToHide()
	signal focusReset()

	visible: dialog.visible
	onVisibleChanged: {
		if(!priv.initialized) {
			priv.initialized = true;
			return;
		}
		dialog.visible = visible;
		if(visible) {
			aboutToShow();
			opened();
		} else {
			aboutToHide();
			closed();
			if(!priv.closedFromQml)
				rejected();
		}
		priv.closedFromQml = false;
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
		priv.closedFromQml = true;
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

	function standardButton(button) {
		return dialog.contentItem.buttonBoxLoader.item.standardButton(button);
	}

	QtObject {
		id: priv
		property bool closedFromQml: true
		property bool initialized: false
	}

	DialogView {
		readonly property Item contents: contentItem.contentsLoader.item
		id: dialog
		title: root.title
		visible: false
		autoClose: root.autoClose
		closable: root.closable
		minimumWidth: nativeDialogMinimumWidth
		minimumHeight: nativeDialogMinimumHeight
		maximumWidth: {
			let contentWidth = contentItem.contentLayout.implicitWidth;
			if(fixedSize)
				return Math.max(contentWidth, minimumWidth);
		}
		maximumHeight: {
			let contentHeight = contentItem.contentLayout.implicitHeight;
			if(fixedSize)
				return Math.max(contentHeight, minimumHeight);
		}

		contentItem: Rectangle {
			property alias contentLayout: contentLayout
			property alias contentsLoader: contentsLoader
			property alias buttonBoxLoader: buttonBoxLoader
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
					active: dialog.visible
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
						onFocusOut: {
							root.focusReset();
							root.forceActiveFocus(Qt.TabFocus);
						}
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
