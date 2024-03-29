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
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0
import OpenTyper.Global 1.0
import "../core"
import "../settings"

CustomDialog {
	property bool focusFromList: false
	property bool forceListFocus: false
	property bool freezeList: false
	signal settingsSynced()
	id: control
	title: qsTr("Settings")
	standardButtons: Dialog.Cancel | Dialog.Ok
	autoClose: false
	contentItem: RowLayout {
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
					name: qsTr("Grades")
					iconName: "medal"
					qmlFileName: "OpenTyper/Grades/GradeSettings.qml"
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
				if(freezeList)
					return;
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
			onActiveFocusChanged: {
				if(forceListFocus)
				{
					forceActiveFocus(Qt.TabFocusReason);
					currentIndex = currentIndex + 0;
					return;
				}
				if(!activeFocus)
				{
					focusFromList = true;
					stack.currentItem.contentItem.children[0].forceActiveFocus(Qt.TabFocusReason);
				}
			}
			onFocusChanged: {
				if(focus)
					forceActiveFocus(Qt.TabFocusReason);
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
			CustomFlickable {
				property url currentComponent
				property int fixedWidth: stack.implicitWidth
				property int fixedHeight: stack.implicitHeight
				id: flickable
				contentWidth: contentItem.children[0].childrenRect.width
				contentHeight: contentItem.children[0].childrenRect.height
				flickableDirection: Flickable.AutoFlickIfNeeded
				showVerticalScrollBar: true
				clip: true
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
								listView.currentItem.forceActiveFocus(Qt.TabFocusReason);
								return;
							}
							focusFromList = false;
							let item = QmlUtils.findFirstControl(children[0]);
							if(item === null)
								children[0].forceActiveFocus(Qt.TabFocusReason);
							else
							{
								let nextItem = item.nextItemInFocusChain();
								if(item.parent === nextItem.parent)
									item.forceActiveFocus(Qt.TabFocusReason);
								else
								{
									if(QmlUtils.itemHasChild(children[0], nextItem))
										nextItem.forceActiveFocus(Qt.TabFocusReason);
									else if(item.activeFocusOnTab)
										item.forceActiveFocus(Qt.TabFocusReason);
									else
										children[0].forceActiveFocus(Qt.TabFocusReason);
								}
							}
						}
					}
				}
				Connections {
					target: QmlUtils
					function onActiveFocusItemChanged() {
						let focusItem = QmlUtils.activeFocusItem;
						if(QmlUtils.itemHasChild(focusScope, focusItem))
							flickable.ensureVisible(focusItem);
						else
							flickable.stopAnimations();
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
		contents.listView.forceActiveFocus(Qt.TabFocusReason);
	}
	onAboutToHide: freezeList = true
	onClosed: freezeList = false
	onAccepted: {
		if(Settings.isFrozen())
			Settings.saveChanges();
		settingsSynced();
	}
	onRejected: {
		if(Settings.isFrozen())
			Settings.discardChanges();
		ThemeEngine.updateTheme();
		settingsSynced();
	}
	onFocusReset: {
		focusFromList = false;
		forceListFocus = true;
		contents.listView.forceActiveFocus(Qt.TabFocusReason);
	}
}
