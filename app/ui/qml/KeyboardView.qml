/*
 * KeyboardView.qml
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
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import OpenTyper 1.0
import "controls"

ColumnLayout {
	property int keySpacing: 7
	readonly property alias layout: layout
	property bool opened: Settings.keyboardVisible()
	id: root
	spacing: keySpacing

	function loadLayout() {
		layout.xkbLayout = BuiltInPacks.keyboardLayoutXkb(BuiltInPacks.keyboardLayoutForPack(Settings.lessonPack()));
	}

	function findKeyInRow(key, rowRepeater) {
		for(var i = 0; i < rowRepeater.count; i++)
		{
			var item = rowRepeater.itemAt(i);
			if(item.type === KeyboardUtils.KeyType_Any && item.text === key.displayText && item.shiftText === key.displayShiftText)
				return item;
		}
		return null;
	}

	function findKey(key) {
		var ret = findKeyInRow(key, rowB);
		if(ret !== null)
			return ret;
		ret = findKeyInRow(key, rowC);
		if(ret !== null)
			return ret;
		ret = findKeyInRow(key, rowD);
		if(ret !== null)
			return ret;
		ret = findKeyInRow(key, rowE);
		return ret;
	}

	function getKeyFromRow(keyText, rowKeys) {
		for(var i = 0; i < rowKeys.length; i++)
		{
			if(rowKeys[i].text === keyText || rowKeys[i].shiftText === keyText ||
				rowKeys[i].displayText === keyText || rowKeys[i].displayShiftText === keyText)
				return findKey(rowKeys[i]);
		}
		return null;
	}

	function getKey(event, normalizeText = false) {
		if(event["key"] === Qt.Key_Backspace)
			return backspace;
		else if(event["key"] === Qt.Key_Tab || event["text"] === "\t")
			return tab;
		else if(event["key"] === Qt.Key_CapsLock)
			return capsLock;
		else if(event["key"] === Qt.Key_Return || event["text"] === "\n")
			return returnKey;
		else if(event["key"] === Qt.Key_Shift)
		{
			if(event["rShift"] === true)
				return rShift;
			else
				return lShift; // TODO: Add support for rShift (pressed key)
		}
		else if(event["key"] === Qt.Key_Control)
			return lCtrl; // TODO: Add support for rCtrl
		else if(event["key"] === Qt.Key_Alt)
			return lAlt;
		else if(event["key"] === Qt.Key_AltGr)
			return rAlt;
		else if(event["key"] === Qt.Key_Space || event["text"] === " ")
			return space;
		if(event["text"] === undefined)
			return;
		var keyText;
		var normalized = StringUtils.normalizeString(event["text"]);
		if(normalizeText && normalized.length > 0)
			keyText = normalized[0].toLowerCase();
		else
			keyText = event["text"].toLowerCase();
		var ret = getKeyFromRow(keyText, layout.rowB);
		if(ret !== null)
			return ret;
		ret = getKeyFromRow(keyText, layout.rowC);
		if(ret !== null)
			return ret;
		ret = getKeyFromRow(keyText, layout.rowD);
		if(ret !== null)
			return ret;
		ret = getKeyFromRow(keyText, layout.rowE);
		if(ret === null && !normalizeText)
			return getKey(event, true);
		return ret;
	}

	function highlightKey(event) {
		var key = getKey(event);
		if(key !== null)
			key.highlighted = true;
	}

	function dehighlightKey(event) {
		var key = getKey(event);
		if(key !== null)
			key.highlighted = false;
	}

	function dehighlightAllKeys() {
		for(var i = 0; i < rowB.count; i++)
			rowB.itemAt(i).highlighted = false;
		for(i = 0; i < rowC.count; i++)
			rowC.itemAt(i).highlighted = false;
		for(i = 0; i < rowD.count; i++)
			rowD.itemAt(i).highlighted = false;
		for(i = 0; i < rowE.count; i++)
			rowE.itemAt(i).highlighted = false;
		backspace.highlighted = false;
		tab.highlighted = false;
		capsLock.highlighted = false;
		returnKey.highlighted = false;
		lShift.highlighted = false;
		rShift.highlighted = false;
		lCtrl.highlighted = false;
		lAlt.highlighted = false;
		space.highlighted = false;
		rAlt.highlighted = false;
		rCtrl.highlighted = false;
	}

	function pressKey(event) {
		var key = getKey(event);
		if(key !== null)
			key.pressed = true;
	}

	function releaseKey(event) {
		var key = getKey(event);
		if(key !== null)
			key.pressed = false;
	}

	function releaseAllKeys() {
		for(var i = 0; i < rowB.count; i++)
			rowB.itemAt(i).pressed = false;
		for(i = 0; i < rowC.count; i++)
			rowC.itemAt(i).pressed = false;
		for(i = 0; i < rowD.count; i++)
			rowD.itemAt(i).pressed = false;
		for(i = 0; i < rowE.count; i++)
			rowE.itemAt(i).pressed = false;
		backspace.pressed = false;
		tab.pressed = false;
		capsLock.pressed = false;
		returnKey.pressed = false;
		lShift.pressed = false;
		rShift.pressed = false;
		lCtrl.pressed = false;
		lAlt.pressed = false;
		space.pressed = false;
		rAlt.pressed = false;
		rCtrl.pressed = false;
	}

	KeyboardLayout {
		property var xkbLayout: ["", ""]
		id: layout
		layoutId: xkbLayout[0]
		variant: xkbLayout[1]
	}

	ColumnLayout {
		property real yScale: Settings.keyboardVisible() ? 1 : 0
		id: mainLayout
		transform: Scale {
			yScale: mainLayout.yScale
			origin.y: mainLayout.height
		}

		// Row E (number row)
		Row {
			spacing: keySpacing

			Repeater {
				id: rowE
				model: layout.rowE
				KeyboardKey {
					keyRow: KeyboardLayout.Row_E
					keyId: index
					layout: root.layout
					text: modelData.displayText
					shiftText: modelData.displayShiftText
					type: modelData.type
				}
			}

			KeyboardKey {
				id: backspace
				keyRow: KeyboardLayout.Row_E
				keyId: rowE.count
				layout: root.layout
				text: ""
				shiftText: "⌫"
				type: KeyboardUtils.KeyType_Backspace
			}
		}

		// Row D
		Row {
			spacing: keySpacing

			KeyboardKey {
				id: tab
				keyRow: KeyboardLayout.Row_D
				keyId: 0
				layout: root.layout
				text: ""
				shiftText: "Tab ⭾"
				type: KeyboardUtils.KeyType_Tab
			}

			Repeater {
				id: rowD
				model: layout.rowD
				KeyboardKey {
					keyRow: KeyboardLayout.Row_D
					keyId: index + 1
					layout: root.layout
					text: modelData.displayText
					shiftText: modelData.displayShiftText
					type: modelData.type
				}
			}
		}

		// Row C (home row)
		Row {
			spacing: keySpacing

			KeyboardKey {
				id: capsLock
				keyRow: KeyboardLayout.Row_C
				keyId: 0
				layout: root.layout
				text: ""
				shiftText: "Caps Lock"
				type: KeyboardUtils.KeyType_CapsLock
			}

			Repeater {
				id: rowC
				model: layout.rowC
				KeyboardKey {
					keyRow: KeyboardLayout.Row_C
					keyId: index + 1
					layout: root.layout
					text: modelData.displayText
					shiftText: modelData.displayShiftText
					type: modelData.type
				}
			}

			KeyboardKey {
				id: returnKey
				keyRow: KeyboardLayout.Row_C
				keyId: rowC.count + 1
				layout: root.layout
				text: ""
				shiftText: "⏎"
				type: KeyboardUtils.KeyType_Return
			}
		}

		// Row B
		Row {
			spacing: keySpacing

			KeyboardKey {
				id: lShift
				keyRow: KeyboardLayout.Row_B
				keyId: 0
				layout: root.layout
				text: ""
				shiftText: "⇧ Shift"
				type: KeyboardUtils.KeyType_LShift
			}

			Repeater {
				id: rowB
				model: layout.rowB
				KeyboardKey {
					keyRow: KeyboardLayout.Row_B
					keyId: index + 1
					layout: root.layout
					text: modelData.displayText
					shiftText: modelData.displayShiftText
					type: modelData.type
				}
			}

			KeyboardKey {
				id: rShift
				keyRow: KeyboardLayout.Row_B
				keyId: rowB.count + 1
				layout: root.layout
				text: ""
				shiftText: "Shift ⇧"
				type: KeyboardUtils.KeyType_RShift
			}
		}

		// Row A
		Row {
			spacing: keySpacing

			KeyboardKey {
				id: lCtrl
				keyRow: KeyboardLayout.Row_A
				keyId: 0
				layout: root.layout
				text: ""
				shiftText: "Ctrl"
				type: KeyboardUtils.KeyType_Ctrl
			}

			KeyboardKey {
				id: lAlt
				keyRow: KeyboardLayout.Row_A
				keyId: 1
				layout: root.layout
				text: ""
				shiftText: "Alt"
				type: KeyboardUtils.KeyType_LAlt
			}

			KeyboardKey {
				id: space
				keyRow: KeyboardLayout.Row_A
				keyId: 2
				layout: root.layout
				text: ""
				shiftText: ""
				type: KeyboardUtils.KeyType_Space
			}

			KeyboardKey {
				id: rAlt
				keyRow: KeyboardLayout.Row_A
				keyId: 3
				layout: root.layout
				text: ""
				shiftText: "Alt"
				type: KeyboardUtils.KeyType_RAlt
			}

			KeyboardKey {
				id: rCtrl
				keyRow: KeyboardLayout.Row_A
				keyId: 4
				layout: root.layout
				text: ""
				shiftText: "Ctrl"
				type: KeyboardUtils.KeyType_Ctrl
			}
		}
	}

	PropertyAnimation {
		id: openAnimation
		target: mainLayout
		property: "yScale"
		to: 1
		duration: 250
		easing.type: Easing.OutCubic
	}

	PropertyAnimation {
		id: closeAnimation
		target: mainLayout
		property: "yScale"
		to: 0
		duration: 250
		easing.type: Easing.InCubic
	}

	// Close button
	Button {
		signal clicked()
		Layout.alignment: Qt.AlignHCenter
		icon.name: opened ? "down" : "up"
		onClicked: {
			opened = !opened
			Settings.setKeyboardVisible(opened);
			if(opened)
			{
				closeAnimation.stop();
				openAnimation.start();
			}
			else
			{
				openAnimation.stop();
				closeAnimation.start();
			}
		}
		onReleased: clicked()

		ToolTip.text: opened ? qsTr("Hide keyboard") : qsTr("Show keyboard")
		ToolTip.visible: hovered
		ToolTip.delay: 500
	}
}
