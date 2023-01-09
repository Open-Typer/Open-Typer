/*
 * LanguageSettings.qml
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
import ".."

// This settings page can be used only in SettingsDialog!
ColumnLayout {
	Label {
		text: qsTr("Language")
		font.bold: true
		font.capitalization: Font.MixedCase
	}
	LanguageList {
		Layout.fillWidth: true
		Layout.fillHeight: true
	}
	Component.onCompleted: {
		// Fill SettingsDialog's Flickable
		width = parent.parent.fixedWidth;
		height = parent.parent.fixedHeight;
	}
}