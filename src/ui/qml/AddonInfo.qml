/*
 * AddonInfo.qml
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
import QtQuick.Layouts 1.2
import OpenTyper 1.0
import OpenTyper.UiComponents 1.0
import "dialogs"

Item {
	property AddonItemModel itemModel
	property AddonModel model: AddonManager.findAddon(itemModel.id)
	readonly property bool installed: model == null ? false : model.installed
	readonly property bool installing: model == null ? false : !model.installed
	signal closed()

	function updateModel() {
		model = AddonManager.findAddon(itemModel.id);
	}

	ColumnLayout {
		id: layout
		anchors.fill: parent

		AccentButton {
			icon.name: "left"
			onClicked: closed()
			Accessible.name: QmlUtils.translateStandardButton("Close")
		}

		RowLayout {
			Image {
				source: itemModel.iconUrl
				sourceSize.width: 64
				sourceSize.height: 64
			}

			Label {
				text: itemModel.name
				font.bold: true
				font.pointSize: 20
			}
		}

		RowLayout {
			Layout.fillWidth: true

			ProgressBar {
				id: progressBar
				Layout.fillWidth: true
				visible: model != null && !model.installed
				value: model == null ? 0 : model.installationProgress
			}

			Item {
				Layout.fillWidth: true
				visible: !progressBar.visible
			}

			AccentButton {
				icon.name: installed ? "delete" : "download"
				text: installed ? qsTr("Uninstall") : (installing ? qsTr("Installing...") : qsTr("Install"))
				enabled: !installing
				onClicked: {
					if(installed)
					{
						AddonManager.uninstallAddon(itemModel.id);
						restartAppMessageBox.open();
					}
					else
						AddonManager.installAddon(itemModel);
					updateModel();
				}
			}
		}

		Label {
			text: itemModel.description
		}

		Item {
			Layout.fillHeight: true
		}
	}

	MessageBox {
		id: restartAppMessageBox
		windowTitle: Qt.application.displayName
		title: qsTr("The application needs to restart.")
		icon: MessageBox.Information
		showShadow: false
		onAboutToHide: QmlUtils.restartApplication()
	}
}
