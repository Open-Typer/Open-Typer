/*
 * AboutDialog.qml
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
import OpenTyper.Ui 1.0
import OpenTyper.UiComponents 1.0

OldCustomDialog {
	//: For example "About Open-Typer" (%1 is the app name)
	windowTitle: qsTr("About %1").arg(Qt.application.displayName)
	standardButtons: Dialog.Ok

	contentComponent: RowLayout {
		spacing: 25

		Image {
			source: "qrc:/res/images/icon.ico"
			sourceSize.width: 60
			sourceSize.height: 60
			Layout.alignment: Qt.AlignTop
		}

		ColumnLayout {
			Label {
				text: Qt.application.displayName
				font.bold: true
			}

			Label {}

			Label {
				text: qsTr("Version: %1").arg(Qt.application.version)
			}

			Label {
				text: qsTr("Revision: %1").arg(QmlUtils.applicationRevision())
			}

			Label {
				readonly property string src: "https://github.com/Open-Typer/Open-Typer"
				text: qsTr("Source code: %1").arg("<a href=\"" + src + "\">" + src + "</a>")
				onLinkActivated: Qt.openUrlExternally(link)
			}

			Label {}

			Label {
				text: "Copyright © 2021-" + QmlUtils.applicationBuildYear() + " adazem009"
			}

			Label {
				text: qsTr("Published with the GNU General Public License.")
			}
		}
	}
}
