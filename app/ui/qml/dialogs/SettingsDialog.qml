/*
 * SettingsDialog.qml
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
import OpenTyper 1.0
import "../controls"
import "../settings"

CustomDialog {
	signal settingsSynced()
	id: control
	windowTitle: qsTr("Settings")
	standardButtons: Dialog.Cancel | Dialog.Ok
	dialog.closePolicy: Popup.NoAutoClose
	contentComponent: RowLayout {
		property alias listView: listView
		property alias stack: stack
		ListView {
			property int previousIndex: -1
			readonly property list<SettingsCategory> categories: [
				SettingsCategory {
					name: qsTr("Language")
					iconName: "language"
					qmlFileName: "settings/LanguageSettings.qml"
				},
				SettingsCategory {
					name: qsTr("Behavior")
					iconName: "tweaks"
					qmlFileName: "settings/BehaviorSettings.qml"
				},
				SettingsCategory {
					name: qsTr("Keyboard")
					iconName: "keyboard"
					qmlFileName: "settings/KeyboardSettings.qml"
				},
				SettingsCategory {
					name: qsTr("Appearance")
					iconName: "appearance"
					qmlFileName: "settings/AppearanceSettings.qml"
				}
			]
			id: listView
			Layout.fillHeight: true
			Binding on implicitWidth {
				value: {
					var maxWidth = 0;
					for(var i = 0; i < listView.contentItem.children.length; i++)
					{
						if(listView.contentItem.children[i].text !== undefined)
						{
							metrics.font = listView.contentItem.children[i].font;
							maxWidth = Math.max(maxWidth, metrics.boundingRect(listView.contentItem.children[i].text).width);
						}
					}
					if(maxWidth == -1)
						return width;
					return maxWidth + 75;
				}
			}
			implicitHeight: contentHeight
			model: categories
			Connections {
				target: AddonApi
				function onSettingsCategoriesChanged() {
					for(var i = 0; i < AddonApi.settingsCategories.length; i++)
					{
						listView.categories.push(AddonApi.settingsCategories[i]);
					}
				}
			}
			delegate: ItemDelegate {
				property string source: "../" + modelData.qmlFileName
				text: modelData.name
				width: listView.width
				icon.name: modelData.iconName
				icon.source: modelData.iconSource
				highlighted: ListView.isCurrentItem
				onClicked: listView.currentIndex = index
			}
			onCurrentIndexChanged: {
				var noTransition = false;
				if(currentIndex == previousIndex)
					noTransition = true;
				if(currentItem == null)
					return;
				if(stack.currentItem == null)
					stack.push(categoryContent);
				else if(currentIndex > previousIndex)
					stack.replace(stack.currentItem, categoryContent, noTransition ? StackView.Immediate : StackView.PushTransition);
				else
					stack.replace(stack.currentItem, categoryContent, noTransition ? StackView.Immediate : StackView.PopTransition);
				stack.currentItem.currentComponent = currentItem.source;
				previousIndex = currentIndex;
			}
			FontMetrics {
				id: metrics
			}
		}
		ToolSeparator { Layout.fillHeight: true }
		Component {
			id: categoryContent
			Flickable {
				property url currentComponent
				property int fixedWidth: stack.implicitWidth
				property int fixedHeight: stack.implicitHeight
				id: flickable
				contentWidth: contentItem.childrenRect.width
				contentHeight: contentItem.childrenRect.height
				flickableDirection: Flickable.AutoFlickIfNeeded
				clip: true
				ScrollBar.vertical: ScrollBar {
					width: 10
					position: flickable.visibleArea.yPosition
					policy: ScrollBar.AlwaysOn
				}
				onCurrentComponentChanged: {
					var component = Qt.createComponent(currentComponent);
					component.createObject(contentItem);
				}
			}
		}
		VerticalStackView {
			id: stack
			Layout.fillWidth: true
			implicitWidth: 600
			implicitHeight: 400
			initialItem: null
		}
	}
	onAboutToShow: {
		contentItem.stack.clear();
		contentItem.listView.currentIndex = -1;
		contentItem.listView.currentIndex = 0;
		Settings.freeze();
	}
	onAccepted: {
		if(Settings.isFrozen())
			Settings.saveChanges();
		settingsSynced();
	}
	onRejected: {
		if(Settings.isFrozen())
			Settings.discardChanges();
		settingsSynced();
	}
}
