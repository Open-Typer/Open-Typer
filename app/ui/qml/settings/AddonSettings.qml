/*
 * AddonSettings.qml
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
import ".."
import "../controls"

// This settings page can be used only in SettingsDialog!
ColumnLayout {
	property bool spinnerRunning: false
	id: root

	AddonListModel {
		id: listModel
		onLoaded: spinnerRunning = false
		Component.onCompleted: reload()
	}

	function reload() {
		spinnerRunning = true;
		listModel.load();
	}

	StackView {
		id: stackView
		Layout.fillWidth: true
		Layout.fillHeight: true
		initialItem: addonListComponent
	}

	Component {
		id: addonListComponent

		Item {
			ColumnLayout {
				anchors.fill: parent

				RowLayout {
					Layout.fillWidth: true

					TextField {
						Layout.fillWidth: true
						placeholderText: qsTr("Search")
					}

					CustomToolButton {
						icon.name: "open"
					}
				}

				TabBar {
					id: bar
					enabled: !spinner.running
					Layout.fillWidth: true
					background: null

					TabButton {
						text: qsTr("Online")
						font.capitalization: Font.MixedCase
					}

					TabButton {
						text: qsTr("My addons")
						font.capitalization: Font.MixedCase
					}
				}

				StackLayout {
					enabled: !spinner.running
					Layout.fillWidth: true
					Layout.fillHeight: true
					currentIndex: bar.currentIndex

					Item {
						id: onlineAddonsTab

						ListView {
							id: onlineList
							anchors.fill: parent
							model: listModel.items
							topMargin: 15
							spacing: 15
							clip: true
							delegate: AddonItem {
								id: addonItem
								width: onlineList.width
								model: modelData
								onClicked: stackView.push(addonInfo)

								Component {
									id: addonInfo
									AddonInfo {
										itemModel: addonItem.model
										onClosed: stackView.pop()
									}
								}
							}
						}
					}
				}

				BusyIndicator {
					id: spinner
					Layout.alignment: Qt.AlignCenter
					running: spinnerRunning
				}
			}
		}
	}

	Component.onCompleted: {
		// Fill SettingsDialog's Flickable
		implicitWidth = parent.parent.fixedWidth;
		implicitHeight = parent.parent.fixedHeight;
	}
}
