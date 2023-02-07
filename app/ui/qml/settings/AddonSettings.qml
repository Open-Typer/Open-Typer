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
import "../controls"

// This settings page can be used only in SettingsDialog!
ColumnLayout {
	id: root

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
		Layout.fillWidth: true
		currentIndex: bar.currentIndex

		Item {
			id: onlineAddonsTab

			ColumnLayout {
				anchors.fill: parent
				Flickable {
					Layout.fillWidth: true
					Layout.fillHeight: true
					contentWidth: addonsLayout.width
					contentHeight: addonsLayout.height
					flickableDirection: Flickable.VerticalFlick
					clip: true
					ScrollBar.vertical: ScrollBar {
						width: 10
						position: flickable.visibleArea.yPosition
						policy: ScrollBar.AlwaysOn
					}

					GridLayout {
						id: addonsLayout
						columnSpacing: 10
						rowSpacing: 10
						columns: 4

						Repeater {
							model: 50
							Frame {
								width: 250
								height: 250
								Label {
									text: "Some addon\nfor Open-Typer"
									font.bold: true
								}
							}
						}
					}
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
