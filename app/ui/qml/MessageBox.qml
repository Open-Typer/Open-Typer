/*
 * MessageBox.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
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
import OpenTyper 1.0

CustomDialog {
	readonly property int noIcon: QmlUtils.NoIcon
	readonly property int information: QmlUtils.Information
	readonly property int warning: QmlUtils.Warning
	readonly property int critical: QmlUtils.Critical
	readonly property int question: QmlUtils.Question
	property string title: ""
	property int icon: noIcon
	standardButtons: Dialog.Ok
	contentComponent: RowLayout {
		Image {
			source: icon == noIcon ? null : QmlUtils.standardIconStr(icon)
		}
		Label {
			text: title
			font.bold: true
			font.pointSize: 14
		}
	}
}
