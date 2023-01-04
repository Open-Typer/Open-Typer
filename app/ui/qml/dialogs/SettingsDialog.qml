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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../controls"
import "../settings"

CustomDialog {
	id: control
	windowTitle: qsTr("Settings")
	standardButtons: Dialog.Cancel | Dialog.Ok
	dialog.closePolicy: Popup.NoAutoClose
	contentComponent: RowLayout {
		ListView {
			property int previousIndex: -1
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
			model: ListModel {
				ListElement {
					name: qsTr("Appearance")
					iconSource: "qrc:/res/images/AppearanceOptions.svg"
					sourceComponent: "../settings/AppearanceSettings.qml"
				}
			}
			delegate: ItemDelegate {
				property string source: sourceComponent
				text: name
				width: listView.width
				icon.source: iconSource
				highlighted: ListView.isCurrentItem
				onClicked: listView.currentIndex = index
			}
			onCurrentIndexChanged: {
				if(stack.currentItem == null)
					stack.push(currentItem.source);
				else if(currentIndex > previousIndex)
					stack.replace(stack.currentItem, currentItem.source, StackView.PushTransition);
				else
					stack.replace(stack.currentItem, currentItem.source, StackView.PopTransition);
				previousIndex = currentIndex;
			}
			FontMetrics {
				id: metrics
			}
		}
		ToolSeparator { Layout.fillHeight: true }
		VerticalStackView {
			id: stack
			Layout.fillWidth: true
			implicitWidth: currentItem.implicitWidth
			implicitHeight: currentItem.implicitHeight
			initialItem: null
		}
	}
	onAboutToShow: Settings.freeze()
	onAccepted: Settings.saveChanges()
	onRejected: Settings.discardChanges()
}
