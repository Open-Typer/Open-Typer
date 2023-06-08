/*
 * CustomDialogButtonBox.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
 * Copyright (C) 2023 - Roker2
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
import OpenTyper.UiComponents 1.0
import OpenTyper.Ui 1.0

AccentButtonBox {
	property color bgColor: ThemeEngine.bgColor
	property int radius: 10
	signal focusOut()
	id: dialogButtonBox
	font.capitalization: Font.MixedCase
	background: Rectangle {
		color: bgColor
		radius: radius
	}
	function retranslateButtons() {
		if(standardButton(Dialog.Ok))
			standardButton(Dialog.Ok).text = QmlUtils.translateStandardButton("OK");
		if(standardButton(Dialog.Open))
			standardButton(Dialog.Open).text = QmlUtils.translateStandardButton("Open");
		if(standardButton(Dialog.Save))
			standardButton(Dialog.Save).text = QmlUtils.translateStandardButton("Save");
		if(standardButton(Dialog.Cancel))
			standardButton(Dialog.Cancel).text = QmlUtils.translateStandardButton("Cancel");
		if(standardButton(Dialog.Close))
			standardButton(Dialog.Close).text = QmlUtils.translateStandardButton("Close");
		if(standardButton(Dialog.Discard))
			standardButton(Dialog.Discard).text = QmlUtils.translateStandardButton("Discard");
		if(standardButton(Dialog.Apply))
			standardButton(Dialog.Apply).text = QmlUtils.translateStandardButton("Apply");
		if(standardButton(Dialog.Reset))
			standardButton(Dialog.Reset).text = QmlUtils.translateStandardButton("Reset");
		if(standardButton(Dialog.RestoreDefaults))
			standardButton(Dialog.RestoreDefaults).text = QmlUtils.translateStandardButton("Restore Defaults");
		if(standardButton(Dialog.Help))
			standardButton(Dialog.Help).text = QmlUtils.translateStandardButton("Help");
		if(standardButton(Dialog.SaveAll))
			standardButton(Dialog.SaveAll).text = QmlUtils.translateStandardButton("Save All");
		if(standardButton(Dialog.Yes))
			standardButton(Dialog.Yes).text = QmlUtils.translateStandardButton("Yes");
		if(standardButton(Dialog.YesToAll))
			standardButton(Dialog.YesToAll).text = QmlUtils.translateStandardButton("Yes to All");
		if(standardButton(Dialog.No))
			standardButton(Dialog.No).text = QmlUtils.translateStandardButton("No");
		if(standardButton(Dialog.NoToAll))
			standardButton(Dialog.NoToAll).text = QmlUtils.translateStandardButton("No to All");
		if(standardButton(Dialog.Abort))
			standardButton(Dialog.Abort).text = QmlUtils.translateStandardButton("Abort");
		if(standardButton(Dialog.Retry))
			standardButton(Dialog.Retry).text = QmlUtils.translateStandardButton("Retry");
		if(standardButton(Dialog.Ignore))
			standardButton(Dialog.Ignore).text = QmlUtils.translateStandardButton("Ignore");
	}
	Connections {
		readonly property Item firstButton: dialogButtonBox.contentChildren[0]
		target: firstButton
        function onActiveFocusChanged(activeFocus) {
            console.log("active focus: ", activeFocus)
            if(!activeFocus)
				focusOut();
		}
	}
	Component.onCompleted: retranslateButtons()
	onStandardButtonsChanged: retranslateButtons()
}
