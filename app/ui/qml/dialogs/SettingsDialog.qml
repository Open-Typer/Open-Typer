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
import QtQuick.Window 2.12
import OpenTyper 1.0
import "../controls"
import "../settings"

CustomDialog {
	property bool focusFromList: false
	property bool forceListFocus: false
	signal settingsSynced()
	id: control
	windowTitle: qsTr("Settings")
	standardButtons: Dialog.Cancel | Dialog.Ok
	dialog.closePolicy: Popup.NoAutoClose
	contentComponent: RowLayout {
		property alias listView: listView
		property alias stack: stack
		ListView {
			property int previousIndex: 0
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
				},
				SettingsCategory {
					name: qsTr("Addons")
					iconName: "add"
					qmlFileName: "settings/AddonSettings.qml"
					visible: !QmlUtils.osWasm()
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
			function updateAddonCategories() {
				for(var i = 0; i < AddonApi.settingsCategories.length; i++)
				{
					listView.categories.push(AddonApi.settingsCategories[i]);
				}
			}
			Connections {
				target: AddonApi
				onSettingsCategoriesChanged: {
					updateAddonCategories();
				}
			}
			delegate: ItemDelegate {
				property string source: "../" + modelData.qmlFileName
				text: modelData.name
				width: listView.width
				visible: modelData.visible
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
				if(noTransition)
				{
					var previousDuration = stack.animationDuration;
					stack.animationDuration = 16;
				}
				else if(currentIndex > previousIndex)
					stack.replace(stack.currentItem, categoryContent, StackView.PushTransition);
				else
					stack.replace(stack.currentItem, categoryContent, StackView.PopTransition);
				stack.currentItem.currentComponent = currentItem.source;
				previousIndex = currentIndex;
				if(noTransition)
					stack.animationDuration = previousDuration;
				forceListFocus = false;
			}
			FontMetrics {
				id: metrics
			}
			Component.onCompleted: updateAddonCategories()
			onActiveFocusChanged: {
				if(forceListFocus)
				{
					forceActiveFocus(Qt.TabFocus);
					currentIndex = currentIndex + 0;
					return;
				}
				if(!activeFocus)
				{
					focusFromList = true;
					stack.currentItem.contentItem.children[0].forceActiveFocus(Qt.TabFocus);
				}
			}
			onFocusChanged: {
				if(focus)
					forceActiveFocus(Qt.TabFocus);
			}
			Keys.onTabPressed: {
				focusFromList = true;
				forceListFocus = false;
				stack.currentItem.contentItem.children[0].forceActiveFocus();
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
					policy: ScrollBar.AsNeeded
				}
				onCurrentComponentChanged: {
					var component = Qt.createComponent(currentComponent);
					var obj = component.createObject(focusScope);
				}
				FocusScope {
					property int fixedWidth: flickable.fixedWidth
					property int fixedHeight: flickable.fixedHeight
					id: focusScope
					onActiveFocusChanged: {
						if(activeFocus)
						{
							if(!focusFromList)
							{
								listView.currentItem.forceActiveFocus(Qt.TabFocus);
								return;
							}
							focusFromList = false;
							let item = QmlUtils.findFirstControl(children[0]);
							if(item === null)
								children[0].forceActiveFocus(Qt.TabFocus);
							else
							{
								let nextItem = item.nextItemInFocusChain();
								if(item.parent === nextItem.parent)
									item.forceActiveFocus(Qt.TabFocus);
								else
								{
									if(QmlUtils.itemHasChild(children[0], nextItem))
										nextItem.forceActiveFocus(Qt.TabFocus);
									else
										children[0].forceActiveFocus(Qt.TabFocus);
								}
							}
						}
					}
				}
			}
		}
		VerticalStackView {
			id: stack
			Layout.fillWidth: true
			implicitWidth: 600
			implicitHeight: 400
			initialItem: categoryContent
		}
	}
	onAboutToShow: {
		Settings.freeze();
		contentItem.listView.forceActiveFocus(Qt.TabFocus);
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
	onActiveFocusChanged: {
		if(activeFocus)
		{
			focusFromList = false;
			forceListFocus = true;
			contentItem.listView.forceActiveFocus(Qt.TabFocus);
		}
	}
}
