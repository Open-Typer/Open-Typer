/*
 * GradingClassConfig.qml
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
import OpenTyper.Grades 1.0
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0

CustomDialog {
	id: root
	windowTitle: qsTr("Classes")
	draggable: false
	fillWindow: true
	standardButtons: Dialog.Close
	onActiveFocusChanged: {
		if(activeFocus)
			contentItem.currentItem.forceActiveFocus(Qt.TabFocus);
	}

	contentComponent: StackView {
		id: stackView
		initialItem: listComponent

		Component {
			id: listComponent

			ColumnLayout {
				readonly property Class currentClass: ClassManager.classes[listView.currentIndex]

				RowLayout {
					Label {
						Layout.fillWidth: true
						text: windowTitle
						font.pointSize: 12
						font.bold: true
					}

					Control {
						id: focusControl
						onActiveFocusChanged: {
							if(activeFocus)
							{
								if(editButton.visible)
									editButton.forceActiveFocus(Qt.TabFocus);
								else
									addButton.forceActiveFocus(Qt.TabFocus);
							}
						}
					}

					CustomToolButton {
						id: editButton
						icon.name: "edit"
						toolTipText: qsTr("Edit")
						visible: listView.currentIndex != -1
						onClicked: stackView.push(configComponent)
					}

					CustomToolButton {
						icon.name: "delete"
						toolTipText: qsTr("Remove")
						visible: listView.currentIndex != -1
						onClicked: ClassManager.removeClass(currentClass)
					}

					CustomToolButton {
						id: addButton
						icon.name: "add"
						text: qsTr("Add class")
						onClicked: ClassManager.createNewClass()
						KeyNavigation.tab: listView.count > 0 ? listView : standardButton(Dialog.Close)
					}
				}

				ListView {
					id: listView
					Layout.fillWidth: true
					Layout.fillHeight: true
					spacing: 15
					model: ClassManager.classes
					currentIndex: -1
					clip: true
					delegate: ListButton {
						width: listView.width
						title: model.name
						text: model.description
						onClicked: listView.currentIndex = index;
						onDoubleClicked: editButton.clicked()
						Keys.onSpacePressed: doubleClicked()
					}
					onActiveFocusChanged: {
						if(activeFocus)
						{
							if(count == 0)
								editButton.forceActiveFocus(Qt.TabFocus);
							else if(currentIndex == -1)
								currentIndex = 0;
						}
					}
				}

				onFocusChanged: {
					if(focus)
						focusControl.forceActiveFocus(Qt.TabFocus);
				}
			}
		}

		Component {
			id: configComponent

			ColumnLayout {
				AccentButton {
					id: backButton
					icon.name: "left"
					onClicked: stackView.pop()
					Accessible.name: QmlUtils.translateStandardButton("Close")
				}

				CustomFlickable {
					id: flickable
					Layout.fillWidth: true
					Layout.fillHeight: true
					contentWidth: contentItem.childrenRect.width
					contentHeight: contentItem.childrenRect.height
					flickableDirection: Flickable.AutoFlickIfNeeded
					showVerticalScrollBar: true
					clip: true

					GradingConfig {
						id: gradingConfig
						currentClass: stackView.get(0).currentClass
					}

					Connections {
						target: QmlUtils
						onActiveFocusItemChanged: {
							let focusItem = QmlUtils.activeFocusItem;
							if(QmlUtils.itemHasChild(gradingConfig, focusItem))
								flickable.ensureVisible(focusItem);
						}
					}
				}

				onFocusChanged: {
					if(focus)
						backButton.forceActiveFocus(Qt.TabFocus);
				}
			}
		}
	}

	Connections {
		target: standardButton(Dialog.Close)
		onActiveFocusChanged: {
			if(!target.activeFocus)
				root.contentItem.currentItem.forceActiveFocus(Qt.TabFocus);
		}
	}
}
