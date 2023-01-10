/*
 * MessageBox.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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
import "../controls"

CustomDialog {
	enum Icon {
		NoIcon,
		Information,
		Warning,
		Critical,
		Question
	}
	property string title: ""
	property int icon: MessageBox.NoIcon
	standardButtons: Dialog.Ok
	contentComponent: RowLayout {
		Image {
			source: icon === MessageBox.NoIcon ? null : QmlUtils.standardIconStr(getIcon(icon))
		}
		Label {
			text: title
			font.bold: true
			font.pointSize: 14
		}
	}

	function getIcon(icon) {
		switch(icon) {
			case MessageBox.NoIcon:
				return QmlUtils.NoIcon;
			case MessageBox.Information:
				return QmlUtils.Information;
			case MessageBox.Warning:
				return QmlUtils.Warning;
			case MessageBox.Critical:
				return QmlUtils.Critical;
			case MessageBox.Question:
				return QmlUtils.Question;
			default:
				return QmlUtils.NoIcon;
		}
	}
}
