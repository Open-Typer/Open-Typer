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
import OpenTyper 1.0

ColumnLayout {
	property int keySpacing: 7
	spacing: keySpacing

	function loadLayout() {
		layout.xkbLayout = BuiltInPacks.keyboardLayoutXkb(BuiltInPacks.keyboardLayoutForPack(Settings.lessonPack()));
	}

	KeyboardLayout {
		property var xkbLayout: ["", ""]
		id: layout
		layoutId: xkbLayout[0]
		variant: xkbLayout[1]
	}

	// Row E (number row)
	RowLayout {
		spacing: keySpacing

		Repeater {
			model: layout.rowE
			KeyboardKey {
				text: modelData.displayText
				shiftText: modelData.displayShiftText
				type: modelData.type
			}
		}

		KeyboardKey {
			text: ""
			shiftText: "⌫"
			type: KeyboardUtils.KeyType_Backspace
		}
	}

	// Row D
	RowLayout {
		spacing: keySpacing

		KeyboardKey {
			text: ""
			shiftText: "Tab ⭾"
			type: KeyboardUtils.KeyType_Tab
		}

		Repeater {
			model: layout.rowD
			KeyboardKey {
				text: modelData.displayText
				shiftText: modelData.displayShiftText
				type: modelData.type
			}
		}
	}

	// Row C (home row)
	RowLayout {
		spacing: keySpacing

		KeyboardKey {
			text: ""
			shiftText: "Caps Lock"
			type: KeyboardUtils.KeyType_CapsLock
		}

		Repeater {
			model: layout.rowC
			KeyboardKey {
				text: modelData.displayText
				shiftText: modelData.displayShiftText
				type: modelData.type
			}
		}

		KeyboardKey {
			text: ""
			shiftText: "⏎"
			type: KeyboardUtils.KeyType_Return
		}
	}

	// Row B
	RowLayout {
		spacing: keySpacing

		KeyboardKey {
			text: ""
			shiftText: "⇧ Shift"
			type: KeyboardUtils.KeyType_LShift
		}

		Repeater {
			model: layout.rowB
			KeyboardKey {
				text: modelData.displayText
				shiftText: modelData.displayShiftText
				type: modelData.type
			}
		}

		KeyboardKey {
			text: ""
			shiftText: "Shift ⇧"
			type: KeyboardUtils.KeyType_RShift
		}
	}

	// Row A
	RowLayout {
		spacing: keySpacing

		KeyboardKey {
			text: ""
			shiftText: "Ctrl"
			type: KeyboardUtils.KeyType_Ctrl
		}

		KeyboardKey {
			text: ""
			shiftText: "Alt"
			type: KeyboardUtils.KeyType_LAlt
		}

		KeyboardKey {
			text: ""
			shiftText: ""
			type: KeyboardUtils.KeyType_Space
		}

		KeyboardKey {
			text: ""
			shiftText: "Alt"
			type: KeyboardUtils.KeyType_RAlt
		}

		KeyboardKey {
			text: ""
			shiftText: "Ctrl"
			type: KeyboardUtils.KeyType_Ctrl
		}
	}
}
